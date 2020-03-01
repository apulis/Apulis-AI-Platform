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

## 全局变量
check_result = {}   ## 检测结果


## 询问检测是否通过
def query(func):

    @wraps(func)
    def do_check(*args, **kwargs):

        # 执行真正的检测函数
        print("\n")
        print("start %s" % (func.__name__))

        print("========================================================================")
        print("========================================================================")

        checked = func(*args, **kwargs)

        print("========================================================================")
        print("========================================================================")

        if not checked:
            ret = raw_input('%s passed? type y/n: ' % (func.__name__))
            if ret.lower() == "y" or ret.lower() == "yes":
                print("passed")
                check_result[func.__name__] = "yes"

            else:
                print("failed")
                check_result[func.__name__] = "no"
        else:
            check_result[func.__name__] = "yes" if checked is True else "no"
            print('%s passed? %s' % (func.__name__, check_result[func.__name__]))

    return do_check


##  执行部署检查
class DeploymentChecker(object):

    def __init__(self):

        self.cluster_config = {}
        self.checker_config = {}

        self.cert = ''
        self.cluster_uername = ''

        self.set_config()
        return

    ## 设置集群配置
    def set_config(self):

        #pdb.set_trace()
        self.cluster_config = init_config(default_config_parameters)

        ## 读取集群配置
        config_file = "../config.yaml"
        if not os.path.exists(config_file):
            parser.print_help()
            print "ERROR: config.yaml does not exist!"
            exit()

        ## 合并集群配置
        with open(config_file) as f:
            merge_config(self.cluster_config, yaml.load(f, Loader=yaml.FullLoader))
            f.close()

        ## 读取集群ID
        if os.path.exists("../deploy/clusterID.yml"):
            with open("../deploy/clusterID.yml") as f:
                tmp = yaml.load(f, Loader=yaml.FullLoader)
                if "clusterId" in tmp:
                    self.cluster_config["clusterId"] = tmp["clusterId"]
        
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

    ## 获取节点信息
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

        t = PrettyTable(['item', 'status'])
        t.align = "l"

        for key, value in sorted(check_result.items()):
            t.add_row([key, value])

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


    ####################################################
    ## 以下区域开始定义用于执行实际检测步骤的各个函数
    ## 所有函数必须按照如下格式命名，否则无法执行:
    ## def check_xxxx
    ## 
    ####################################################

    @query
    def check_01_sudo(self):

        cmd = 'sudo -l -U %s' % (self.cluster_config["admin_username"])
        expect = "(ALL)NOPASSWD:ALL"
        
        def check_expect(output):
            output = output.replace(" ", "")

            if expect not in output:
                return False
            else:
                pass  

            return True

        for host in self.get_all_nodes():
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)
        
            if not passed:
                return False
            else:
                pass
              
        return True

    @query
    def check_02_hostname(self):

        cmd = 'sudo hostname'
        expect = ""
                
        def check_expect(output):
            output = output.strip()

            if expect != output:
                return False
            else:
                pass  

            return True

        #pdb.set_trace()
        for host in self.get_all_nodes_hostname():
            expect = host
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return False
            else:
                pass

        return True
    
    @query
    def check_03_dns(self):

        cmd = 'sudo ping -c 3 '
        expect = "time="
                
        def check_expect(output):
            output = output.strip()

            if expect not in output:
                return False
            else:
                pass  

            return True

        #pdb.set_trace()
        for host in self.get_all_nodes():
            passed = self.cluster_ssh_cmd_and_check(host, cmd + host, check_expect)  
        
            if not passed:
                return False
            else:
                pass
            

        return True

    @query
    def check_04_nvidia_driver(self):

        expect1 = "NVIDIA-SMI"
        expect2 = "Driver Version"

        def check_expect(output):
            output = output.strip()

            if expect1 not in output:
                return False
            else:
                pass  

            if expect2 not in output:
                return False
            else:
                pass  

            return True

        for host in self.get_worker_nodes():
            cmd = 'sudo nvidia-docker run --rm dlws/cuda nvidia-smi'
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return False
            else:
                pass     

            cmd = 'sudo docker run --rm -ti dlws/cuda nvidia-smi'
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return False
            else:
                pass    

        return True

    @query
    def check_05_nfs(self):

        #pdb.set_trace()
        share_name = fetch_dictionary(self.cluster_config, ["mountpoints", "nfsshare1", "filesharename"])
        if share_name is None:
            return False
        else:
            pass

        cmd = 'sudo df -h | grep %s' % (share_name)
        expect = share_name
        
        def check_expect(output):
            if expect not in output:
                return False
            else:
                pass  

            return True

        for host in self.get_worker_nodes():
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return False
            else:
                pass       

        return True

    @query
    def check_06_k8s(self):

        global check_result

        func_name = "check_06_k8s"
        cmd = 'export KUBECONFIG=/etc/kubernetes/admin.conf && kubectl get po --all-namespaces'
        components_list = [

            ## k8s组件
            "custom-metrics-apiserver",
            "prometheus-operator",
            "device-plugin",
            "cloud-collectd-node-agent",
            "coredns",
            "etcd",
            "grafana",
            "job-exporter",
            "kube-apiserver",
            "kube-controller-manager",
            "kube-proxy",
            "mysql",
            "node-exporter",
            "prometheus-deployment",
            "watchdog",
            "weave-net",

            ## 应用组件
            "jobmanager",
            "nginx",
            "restfulapi",
            "webui"
        ]

        expect_name   = ""
        expect_status = "running"
        expect_restart = "3"
        svc_states = []

        def check_expect(output):

            pod_state_mapping = {}
            pod_list = output.split("\n")
            #pdb.set_trace()

            ## 服务名称
            for svc_name in components_list:

                ## 可能有多个
                svc_pod_count = 0
                check_item_name = ""

                for pod_line in pod_list:

                    if svc_name in pod_line:

                        svc_pod_count += 1

                        svc_states = pod_line.split()
                        check_item_name = func_name + "_" + svc_name + "_" + str(svc_pod_count)

                        if len(svc_states) < 6 or svc_states[3].lower() != expect_status:
                            check_result[check_item_name] = "no"
                            print('%s passed? %s' % (check_item_name, check_result[check_item_name]))
                            continue
                        else:
                            pass  

                        if len(svc_states) < 6 or svc_states[4].lower() > expect_restart:
                            check_result[check_item_name] = "no"
                            print('%s passed? %s' % (check_item_name, check_result[check_item_name]))
                            continue
                        else:
                            pass 

                        check_result[check_item_name] = "yes"
                        print('%s passed? %s' % (check_item_name, check_result[check_item_name]))

                    else:
                        pass 

                ## 搜索完毕，判断是否找到服务
                if svc_pod_count == 0:
                    ## 未找到服务
                    check_item_name = func_name + "_" + svc_name
                    check_result[check_item_name] = "no"
                    print('%s passed? %s' % (check_item_name, check_result[check_item_name]))

                else:
                    ## 找到服务
                    ## 服务状态在 已在for循环中处理过
                    pass

            return True


        for host in self.get_master_nodes():
            self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
     
        return True
    

def main():
    # the program always run at the current directory.
    dirpath = os.path.dirname(os.path.abspath(os.path.realpath(__file__)))
    os.chdir(dirpath)

    parser = argparse.ArgumentParser(prog='check.py',
        formatter_class=argparse.RawDescriptionHelpFormatter
        )

    '''parser.add_argument("--skip-error",
        help = "检测不通过继续执行后续检测步骤.",
        action="store_true"
        )'''

    #args = parser.parse_args()
    #command = args.command
    #nargs = args.nargs

    config = init_config(default_config_parameters)
    print(config["ssh_cert"], config["admin_username"])

    checker = DeploymentChecker()
    checker.start_check()
    checker.print_summary()

    return

if __name__ == '__main__': 
    main()
