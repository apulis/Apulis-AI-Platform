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
from checker import query, Checker


##  执行部署检查
class K8SChecker(Checker):

    def __init__(self):
        super(K8SChecker, self).__init__()
        return

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
                return (False, [])
            else:
                pass
              
        return (True, [])

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
                return (False, [])
            else:
                pass

        return (True, [])
    
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
                return (False, [])
            else:
                pass
            

        return (True, [])

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

        nodes_data = self.get_nodes_info("worker") 
        for host in nodes_data.keys():
    
            host_info = nodes_data[host]
            worker_type = host_info["type"]

            if worker_type is None or worker_type.lower() != "gpu":
                del nodes_data[host]
            else:
                pass

        if len(nodes_data) == 0:
            return (False, ["no gpu worker"])
        else:
            pass

        
        for host in nodes_data.keys():

            cmd = 'sudo nvidia-docker run --rm dlws/cuda nvidia-smi'
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return (False, ["sudo nvidia-docker run --rm dlws/cuda nvidia-smi: failed!"])
            else:
                pass     

            cmd = 'sudo docker run --rm -ti dlws/cuda nvidia-smi'
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return (False, ["sudo docker run --rm -ti dlws/cuda nvidia-smi: failed!"])
            else:
                pass    

        return (True, [])

    @query
    def check_04_npu_driver(self):
    
        expect1 = "NPU ID"
        expect2 = "Chip ID"

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

        nodes_data = self.get_nodes_info("worker") 
        for host in nodes_data.keys():
    
            host_info = nodes_data[host]
            worker_type = host_info["type"]

            if worker_type is None or worker_type.lower() != "npu":
                del nodes_data[host]
            else:
                pass

        if len(nodes_data) == 0:
            return (False, ["no npu worker"])
        else:
            pass

        for host in nodes_data.keys():

            cmd = 'sudo /usr/local/sbin/npu-smi info -t common -i 255 | grep "NPU ID" -A10'
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
        
            if not passed:
                return (False, ['sudo /usr/local/sbin/npu-smi info -t common -i 255 | grep "NPU ID" -A10' + ": failed"])
            else:
                pass     

        return (True, [])
    

    @query
    def check_05_nfs(self):

        #pdb.set_trace()
        share_name = self.cluster_config["mountpoints"]["nfsshare1"]["filesharename"]
        server_name = self.cluster_config["mountpoints"]["nfsshare1"]["server"]

        if share_name is None or server_name is None:
            return (False, [])
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
            if server_name not in host:
                passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
            
                if not passed:
                    return (False, [])
                else:
                    pass       
            else:
                pass

        return (True, [])

    @query
    def check_06_k8s(self):

        func_name = "check_06_k8s"
        cmd = 'export KUBECONFIG=/etc/kubernetes/admin.conf && kubectl get po --all-namespaces'
        components_list = [

            ## k8s组件
            #"custom-metrics-apiserver",
            "prometheus-operator",
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

        nodes_data = self.get_nodes_info("worker") 
        for host in nodes_data.keys():

            host_info = nodes_data[host]
            worker_type = host_info["type"]

            if worker_type.lower() == "npu":
                components_list.append("a910-device-plugin")

            elif worker_type.lower() == "gpu":
                components_list.append("nvidia-device-plugin")
                
            else:
                pass

        expect_name   = ""
        expect_status = "running"
        expect_restart = 30
        svc_states = []
        reasons = []
        passed_list  = []

        def check_expect(output):

            pod_state_mapping = {}
            pod_list = output.split("\n")
            #pdb.set_trace()

            ## 服务名称
            for svc_name in components_list:

                ## 可能有多个
                svc_pod_count = 0
                check_item_name = ""
                found = False

                for pod_line in pod_list:

                    if svc_name in pod_line:

                        found = True
                        svc_pod_count += 1

                        svc_states = pod_line.split()
                        check_item_name = func_name + "_" + svc_name + "_" + str(svc_pod_count)

                        if len(svc_states) < 6 or svc_states[3].lower() != expect_status:
                            passed_list.append(False)
                            reasons.append(check_item_name + " status is not " + expect_status)
                            continue
                        else:
                            pass  

                        if len(svc_states) < 6 or int(svc_states[4].lower()) > expect_restart:
                            passed_list.append(False)
                            reasons.append(check_item_name + " restarts more than " + str(expect_restart))
                            continue
                        else:
                            pass 

                        passed_list.append(True)
                    else:
                        pass 

                ## 搜索完毕，判断是否找到服务
                if found is True:
                    pass

                else:
                    ## 找不到服务
                    passed_list.append(False)
                    reasons.append(svc_name + " is not started")

            for state in passed_list:
                if state is False:
                    return False
                else:
                    pass

            return True


        for host in self.get_master_nodes():
            passed = self.cluster_ssh_cmd_and_check(host, cmd, check_expect)  
            if not passed:
                return (False, reasons)
            else:
                pass     
     
        return (True, [])
    

def main():
    
    checker = K8SChecker()
    checker.start_check()
    checker.print_summary()

    return

if __name__ == '__main__': 
    main()