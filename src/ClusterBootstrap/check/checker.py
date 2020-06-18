#!/usr/bin/python
# -*- coding: UTF-8 -*- 

import json
import os
import time
import datetime
import argparse
import uuid
import subprocess
import sys
import textwrap
import re
import math
import distutils.dir_util
import distutils.file_util
import shutil
import random
import glob
import copy
import numbers
import requests

from os.path import expanduser

import yaml
from jinja2 import Environment, FileSystemLoader, Template
import base64
import tempfile

from shutil import copyfile, copytree
import urllib
import socket

sys.path.append("../storage/glusterFS")
from GlusterFSUtils import GlusterFSJson

sys.path.append("../")
import utils   # path: src/ClusterBootstrap/utils.py

sys.path.append("../../utils")
from DockerUtils import push_one_docker, build_dockers, push_dockers, run_docker, find_dockers, build_docker_fullname, copy_from_docker_image, configuration, get_reponame
import k8sUtils
from config import config as k8sconfig

sys.path.append("../../docker-images/glusterfs")
import launch_glusterfs
import az_tools
import acs_tools


from params import default_config_parameters, scriptblocks
from ConfigUtils import *

import pdb
from functools import wraps

import types
from prettytable import PrettyTable

import os.path
import sys

sys.path.append("../")
import deploy


## 全局变量
check_result = {}   ## 检测结果
check_reason = {}   ## 失败原因


## 询问检测是否通过
def query(func):

    @wraps(func)
    def do_check(*args, **kwargs):

        # 执行真正的检测函数
        print("\n")
        print("start %s" % (func.__name__))

        print("========================================================================")
        print("========================================================================")

        checked, reasons = func(*args, **kwargs)

        print("========================================================================")
        print("========================================================================")

        check_result[func.__name__] = "yes" if checked is True else "no"
        check_reason[func.__name__] = reasons

        print('%s passed? %s' % (func.__name__, check_result[func.__name__]))

    return do_check

## service信息
class ServiceManager(object):

    def __init__(self, cluster_config):

        self.service_to_nodes = {}
        self.service_to_labels = {}
        self.cluster_config = cluster_config

        self.init()
        return

    def get_nodes(self, service_name):
        if service_name in self.service_to_nodes:
            return self.service_to_nodes[service_name]
        else:
            pass

        return [] 

    def init(self):

        servicelists = {}
        servicedic = deploy.get_all_services()
        labels = deploy.fetch_config(self.cluster_config, ["kubelabels"])

        for service, serviceinfo in servicedic.iteritems():
            servicename = deploy.get_service_name(servicedic[service])

            if (not service in labels) and (not servicename in labels) and "default" in labels and (not servicename is None):
                print "not in: {},{}\n".format(service, serviceinfo)
                labels[servicename] = labels["default"]
            else:
                pass

        servicelists = labels
        #print(servicelists)

        for service in servicelists:
            self.service_to_labels[service] = service+"=active"
            self.service_to_nodes[service] = deploy.get_node_lists_for_service(service)
            print(service)

        #print(self.service_to_labels)
        #print(self.service_to_nodes)

        return


##  执行部署检查
class Checker(object):

    def __init__(self):

        self.cluster_config = {}
        self.checker_config = {}

        self.cert = ''
        self.cluster_uername = ''

        self.set_config()
        self.sm = ServiceManager(self.cluster_config)
        return

    ## 设置集群配置
    def set_config(self):

        #pdb.set_trace()
        self.cluster_config = deploy.get_config()

        
        ## 导入key信息
        self.load_sshkey()

        return

    def load_sshkey(self):

        if os.path.isfile("../deploy/sshkey/id_rsa"):
            self.cluster_config["ssh_cert"] = "../deploy/sshkey/id_rsa"
        else:
            pass

        if "ssh_cert" in self.cluster_config:
            self.cluster_config["ssh_cert"] = expand_path(self.cluster_config["ssh_cert"])
        else:
            pass

        self.cluster_config["etcd_user"] = self.cluster_config["admin_username"]
        self.cluster_config["nfs_user"] = self.cluster_config["admin_username"]
        self.cluster_config["kubernetes_master_ssh_user"] = self.cluster_config["admin_username"]

        print(self.cluster_config["ssh_cert"], self.cluster_config["admin_username"])
        return

    ## 更新集群配置，譬如增加映射等等
    def process_config(self):

        apply_config_mapping(self.cluster_config, default_config_mapping)
        update_one_config(self.cluster_config, "coreosversion",["coreos","version"], basestring, coreosversion)
        update_one_config(self.cluster_config, "coreoschannel",["coreos","channel"], basestring, coreoschannel)
        update_one_config(self.cluster_config, "coreosbaseurl",["coreos","baseurl"], basestring, coreosbaseurl)

        if self.cluster_config["coreosbaseurl"] == "":
            self.cluster_config["coreosusebaseurl"] = ""
        else:
            self.cluster_config["coreosusebaseurl"] = "-b "+self.cluster_config["coreosbaseurl"]

        for (cf, loc) in [('master', 'master'), ('worker', 'kubelet')]:
            exec("self.cluster_config[\"%s_predeploy\"] = os.path.join(\"./deploy/%s\", self.cluster_config[\"pre%sdeploymentscript\"])" % (cf, loc, cf))
            exec("self.cluster_config[\"%s_filesdeploy\"] = os.path.join(\"./deploy/%s\", self.cluster_config[\"%sdeploymentlist\"])" % (cf, loc, cf))
            exec("self.cluster_config[\"%s_postdeploy\"] = os.path.join(\"./deploy/%s\", self.cluster_config[\"post%sdeploymentscript\"])" % (cf, loc, cf))
        
        self.cluster_config["webportal_node"] = None if len(get_node_lists_for_service("webportal"))==0 else get_node_lists_for_service("webportal")[0]

        if ("influxdb_node" not in self.cluster_config):
            self.cluster_config["influxdb_node"] = self.cluster_config["webportal_node"]

        if ("elasticsearch_node" not in self.cluster_config):
            self.cluster_config["elasticsearch_node"] = self.cluster_config["webportal_node"]

        if ("mysql_node" not in self.cluster_config):
            self.cluster_config["mysql_node"] = None if len(get_node_lists_for_service("mysql"))==0 else get_node_lists_for_service("mysql")[0]
        
        if ("host" not in self.cluster_config["prometheus"]):
            self.cluster_config["prometheus"]["host"] = None if len(get_node_lists_for_service("prometheus"))==0 else get_node_lists_for_service("prometheus")[0]
            
        ## 更新docker镜像
        #print "Config:\n{0}\n".format(config)
        if self.cluster_config["kube_custom_scheduler"] or self.cluster_config["kube_custom_cri"]:

            if "container" not in self.cluster_config["dockers"]:
                self.cluster_config["dockers"]["container"] = {}

            if "hyperkube" not in self.cluster_config["dockers"]["container"]:
                self.cluster_config["dockers"]["container"]["hyperkube"] = {}
            # config["dockers"]["container"]["hyperkube"]["fullname"] = config["worker-dockerregistry"] + config["dockerprefix"] + "kubernetes:" + config["dockertag"]

        return

    def get_nodes_by_service(self, name):
        return self.sm.get_nodes(name)

    ## 执行所有check_xxx函数
    def start_check(self):

        for name in sorted(dir(self)):
            if name.startswith('check_'):
                method = getattr(self, name)
                method()
            else:
                pass

        return

    ## 列出所有worker节点
    def get_worker_nodes(self):

        #pdb.set_trace()
        return self.get_nodes_from_config("worker")

    ## 列出所有master节点
    def get_master_nodes(self):

        #pdb.set_trace()
        return self.get_nodes_from_config("infrastructure")

    ## 列出所有节点
    def get_all_nodes(self):
        return self.get_master_nodes() + self.get_worker_nodes()

    ## 列出所有节点主机名
    def get_all_nodes_hostname(self):

        hosts = []
        for nodename in self.cluster_config["machines"]:
            hosts.append(nodename)

        return hosts

    ## 获取节点信息 -- 仅返回域名信息
    def get_nodes_from_config(self, machinerole):

        machinerole = "infrastructure" if machinerole == "infra" else machinerole
        if "machines" not in self.cluster_config:
            return []

        else:

            domain = self.get_domain()
            Nodes = []

            for nodename in self.cluster_config["machines"]:

                nodeInfo = self.cluster_config["machines"][nodename]
                if "role" in nodeInfo and nodeInfo["role"]==machinerole:
                    if len(nodename.split("."))<3:
                        Nodes.append(nodename+domain)
                    else:
                        Nodes.append(nodename)

            return sorted(Nodes)

    
    ## 获取节点信息 -- 仅返回域名信息
    def get_nodes_info(self, machinerole):

        machinerole = "infrastructure" if machinerole == "infra" else machinerole
        if "machines" not in self.cluster_config:
            return {}

        else:

            domain = self.get_domain()
            Nodes = {}

            for nodename in self.cluster_config["machines"]:

                nodeInfo = self.cluster_config["machines"][nodename]

                if "role" in nodeInfo and nodeInfo["role"]==machinerole:

                    if len(nodename.split(".")) < 3:
                        Nodes[nodename+domain] = nodeInfo
                    else:
                        Nodes[nodename] = nodeInfo

                else:
                    pass

            return Nodes
        
        return {}

    ## 获取域名
    def get_domain(self):

        if "network" in self.cluster_config and "domain" in self.cluster_config["network"] and len(self.cluster_config["network"]["domain"]) > 0 :
            domain = "."+self.cluster_config["network"]["domain"]
        else:
            domain = ""

        return domain

    # 打印检测结果
    def print_summary(self):

        print("========================================================================")
        print("========================================================================")
        print("                                                                        ")
        print("check result: ") 

        global check_result
        global check_reason

        t = PrettyTable(['item', 'status', 'reason'])
        t.align = "l"

        for key, value in sorted(check_result.items()):
            reason = "\n".join(check_reason[key])
            t.add_row([key, value, reason])

        print(t)
        return

    ## 封装ssh函数
    def cluster_ssh_cmd(self, host, cmd):

        print("【" + host + "】:")

        if len(cmd)==0:
            return "";
        else:
            pass
            
        execmd = """ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile=/dev/null" -i %s "%s@%s" '%s' """ % (
            self.cluster_config["ssh_cert"], 
            self.cluster_config["admin_username"], 
            host, cmd )
        print(execmd)

        try:
            output = subprocess.check_output(execmd, shell=True)

        except subprocess.CalledProcessError as e:
            output = "Return code: " + str(e.returncode) + ", output: " + e.output.strip()

        print(output)
        return output

    ## 封装ssh函数
    ## 对SSH结果执行check_func检查，检查通过返回True，否则返回False
    def cluster_ssh_cmd_and_check(self, host, cmd, check_func):

        print("【" + host + "】:")

        if len(cmd)==0:
            return "";
        else:
            pass
            
        execmd = """ssh -o "StrictHostKeyChecking no" -o "UserKnownHostsFile=/dev/null" -i %s "%s@%s" '%s' """ % (
            self.cluster_config["ssh_cert"], 
            self.cluster_config["admin_username"], 
            host, cmd )
        print(execmd)

        try:
            output = subprocess.check_output(execmd, shell=True)
        except subprocess.CalledProcessError as e:
            output = "Return code: " + str(e.returncode) + ", output: " + e.output.strip()
            
        print(output)
        return check_func(output)



## 工具函数
def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False
