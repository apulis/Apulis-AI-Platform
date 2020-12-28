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
class ServiceChecker(Checker):

    def __init__(self):
        super(ServiceChecker, self).__init__()
        return

    def __check_ports(self, service_name, service_ports):

        cmd = 'sudo lsof -Pi :%s -sTCP:LISTEN -t | wc -l '
        expect = ""
        
        def check_expect(output):

            if is_number(output) is False:
                return False
            else:
                pass

            lines = int(output)
            if lines < 1:
                return False
            else:
                pass  

            return True

        node_list = self.get_nodes_by_service(service_name)

        if service_name.lower() == "etcd":
            node_list = self.get_master_nodes()
        else:
            pass

        for host in node_list:
            for port in service_ports:
                passed = self.cluster_ssh_cmd_and_check(host, cmd % port, check_expect)
        
                if not passed:
                    return (False, [service_name + " not listening on port: " + str(port)])
                else:
                    pass

        return (True, [])

    ####################################################
    ## 以下区域开始定义用于执行实际检测步骤的各个函数
    ## 所有函数必须按照如下格式命名，否则无法执行:
    ## def check_xxxx
    ## 
    ####################################################
    @query
    def check_01_nginx(self):

        service_name = "nginx"
        service_ports = [443, 80]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    
    @query
    def check_02_restfulapi(self):

        service_name = "restfulapi"
        service_ports = [5000]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_03_jobmanager(self):

        service_name = "jobmanager"
        service_ports = [i + 9200 for i in range(0,9)]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_04_webui(self):

        service_name = "webui3"
        service_ports = [self.cluster_config["webuiport" ] if "webuiport" in self.cluster_config else 3080]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_05_etcd(self):

        service_name = "etcd"
        service_ports = [2380, 2381]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_06_grafana(self):

        service_name = "grafana"
        service_ports = [self.cluster_config["grafana"]["port"]]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_07_mysql(self):

        service_name = "mysql"
        service_ports = [self.cluster_config["mysql_port"]]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_08_node_exporter(self):

        service_name = "node-exporter"
        service_ports = [self.cluster_config["node-exporter"]["port"]]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_09_prometheus(self):

        service_name = "prometheus"
        service_ports = [self.cluster_config["prometheus"]["port"]]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

    @query
    def check_10_gpu_reporter(self):

        service_name = "prometheus"
        service_ports = [self.cluster_config["prometheus"]["reporter"]["port"]]

        cmd = ''
        expect = ""
        
        ports_ready, reasons = self.__check_ports(service_name, service_ports)
        if ports_ready is not True:
            return (ports_ready, reasons)
        else:
            pass
              
        return (True, [])

## 工具函数
def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False
    
def main():

    checker = ServiceChecker()
    checker.start_check()
    checker.print_summary()

    return

if __name__ == '__main__': 
    main()


