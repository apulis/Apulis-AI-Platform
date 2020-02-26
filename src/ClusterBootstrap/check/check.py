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


## 询问检测是否通过
def query(func):

    @wraps(func)
    def do_check(*args, **kwargs):

        # 执行真正的检测函数
        func(*args, **kwargs)

        ret = input('%s passed? yes or no. \n' % (func.__name__))
        if ret.lower() == "y" or ret.lower() == "yes":
            print("passed")
        else:
            print("failed")
            sys.exit()

    return do_check


##  执行部署检查
class DeploymentChecker(object):

    def __init__(self):

        self.cluster_config = {}
        self.checker_config = {}

        self.cluster_config_file = cluster_config_file
        self.checker_config_file = checker_config_file

        self.cert = ''
        self.cluster_uername = ''

        self.set_config()
        return

    ## 设置集群配置
    def set_config(self):

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
        
        ## ssh拷贝文件到临时目录


        ## 更新集群镜像配置、influx db配置、elastic search配置
        if os.path.exists("../deploy/clusterID.yml"):
            #update_config()
            pass
        else:
            #apply_config_mapping(config, default_config_mapping)
            #update_docker_image_config()
            pass

        ## 更新集群配置
        self.process_config()
        if "ssh_cert" not in config and os.path.isfile("./deploy/sshkey/id_rsa"):
            config["ssh_cert"] = "./deploy/sshkey/id_rsa"
        if "ssh_cert" in config:
            config["ssh_cert"] = expand_path(config["ssh_cert"])
            
        config["etcd_user"] = config["admin_username"]
        config["nfs_user"] = config["admin_username"]
        config["kubernetes_master_ssh_user"] = config["admin_username"]

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

    

    @query
    def check_hostname(self):
        return





@check
def check_hostname(host):
    cmd = "hostnamectl"
    ssh_exec_cmd("pengbifeng", "pengbifeng", host, cmd)
    return



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

    args = parser.parse_args()
    #command = args.command
    #nargs = args.nargs

    config = init_config(default_config_parameters)]
    print(config["ssh_cert"], config["admin_username"])



    return




if __name__ == '__main__': 
    main()
