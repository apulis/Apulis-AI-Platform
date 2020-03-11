#!/usr/bin/env python

import collectd

import json
import os
import subprocess
import sys


import yaml

import re

import pycurl
from StringIO import StringIO
import traceback


# Execute a ssh cmd 
# Return the output of the remote command to local
def local_ssh_exec_cmd_with_output(cmd, supressWarning = False):

    if len(cmd)==0:
        return "";

    if supressWarning:
        cmd += " 2>/dev/null"

    execmd = cmd
    print(execmd)

    try:
        output = subprocess.check_output( execmd, shell=True )
    except subprocess.CalledProcessError as e:
        output = "Return code: " + str(e.returncode) + ", output: " + e.output.strip()
        
    #print output
    return output

def curl_get(url):
    curl = pycurl.Curl()
    curl.setopt(pycurl.URL, url)
    curl.setopt(pycurl.SSL_VERIFYPEER, 1)
    curl.setopt(pycurl.SSL_VERIFYHOST, 0)
    curl.setopt(pycurl.CAINFO, "/etc/kubernetes/ssl/ca.pem")
    curl.setopt(pycurl.SSLKEYTYPE, "PEM")
    curl.setopt(pycurl.SSLKEY, "/etc/kubernetes/ssl/apiserver-key.pem")
    curl.setopt(pycurl.SSLCERTTYPE, "PEM")
    curl.setopt(pycurl.SSLCERT, "/etc/kubernetes/ssl/apiserver.pem")
    curl.setopt(curl.FOLLOWLOCATION, True)
    buff = StringIO()
    curl.setopt(pycurl.WRITEFUNCTION, buff.write)
    curl.perform()
    responseStr = buff.getvalue()
    curl.close()
    return responseStr



def configure(conf):
        collectd.info('Configured with')


def read(data=None):
        vl = collectd.Values(type='gauge')
        vl.plugin = 'kubernetes'
        try:
            rsset = json.loads(curl_get(os.environ['K8SAPI']+"/apis/apps/v1/replicasets"))

            if "items" in rsset:
                for rs in rsset["items"]:
                    if "metadata" in rs and "name" in rs["metadata"] and "status" in rs:
                            vl.plugin_instance = rs["metadata"]["name"]

                            if "availableReplicas" in rs["status"]:
                                numberAvailable = float(rs["status"]["availableReplicas"])
                            else:
                                numberAvailable = 0

                            if "replicas" in rs["status"]:
                                desiredNumber = float(rs["status"]["replicas"])
                            else:
                                desiredNumber = 0

                            if "readyReplicas" in rs["status"]:
                                readyNumber = float(rs["status"]["readyReplicas"])
                            else:
                                readyNumber = 0

                            collectd.info('kubernetes plugin: replicaset "%s" with values: %f %f %f' % (rs["metadata"]["name"],desiredNumber,numberAvailable,readyNumber))
                            if desiredNumber > 0 and desiredNumber == readyNumber and desiredNumber == numberAvailable:
                                res = 0
                            else:
                                res = 1
                            vl.dispatch(values=[float(res)])

            rsset = json.loads(curl_get(os.environ['K8SAPI']+"/apis/extensions/v1/ReplicationController"))

            if "items" in rsset:
                for rs in rsset["items"]:
                    if "metadata" in rs and "name" in rs["metadata"] and "status" in rs:
                            vl.plugin_instance = rs["metadata"]["name"]

                            if "availableReplicas" in rs["status"]:
                                numberAvailable = float(rs["status"]["availableReplicas"])
                            else:
                                numberAvailable = 0

                            if "replicas" in rs["status"]:
                                desiredNumber = float(rs["status"]["replicas"])
                            else:
                                desiredNumber = 0

                            if "readyReplicas" in rs["status"]:
                                readyNumber = float(rs["status"]["readyReplicas"])
                            else:
                                readyNumber = 0

                            collectd.info('kubernetes plugin: ReplicationController "%s" with values: %f %f %f' % (rs["metadata"]["name"],desiredNumber,numberAvailable,readyNumber))
                        
                            if desiredNumber > 0 and desiredNumber == readyNumber and desiredNumber == numberAvailable:
                                res = 0
                            else:
                                res = 1
                            vl.dispatch(values=[float(res)])


            dpset = json.loads(curl_get(os.environ['K8SAPI']+"/apis/apps/v1/daemonsets"))
            if "items" in dpset:
                for dp in dpset["items"]:
                    if "metadata" in dp and "name" in dp["metadata"] and "status" in dp:
                            vl.plugin_instance = dp["metadata"]["name"]
                            if "numberAvailable" in dp["status"]:
                                numberAvailable = float(dp["status"]["numberAvailable"])
                            else:
                                numberAvailable = 0

                            if "desiredNumberScheduled" in dp["status"]:
                                desiredNumber = float(dp["status"]["desiredNumberScheduled"])
                            else:
                                desiredNumber = 0

                            if "numberReady" in dp["status"]:
                                readyNumber = float(dp["status"]["numberReady"])
                            else:
                                readyNumber = 0

                            collectd.info('kubernetes plugin: deployment "%s" with values: %f %f %f' % (dp["metadata"]["name"],desiredNumber,numberAvailable,readyNumber))
                        
                            if desiredNumber > 0 and desiredNumber == readyNumber and desiredNumber == numberAvailable:
                                res = 0
                            else:
                                res = 1
                            vl.dispatch(values=[float(res)])

        except:
            exc_type, exc_value, exc_traceback = sys.exc_info()
            print "*** print_tb:"
            traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
            print "*** print_exception:"
            traceback.print_exception(exc_type, exc_value, exc_traceback,
                                      limit=2, file=sys.stdout)
            print "*** print_exc:"
            traceback.print_exc()

        try:
            used_gpus = 0
            pods = json.loads( curl_get(os.environ['K8SAPI']+"/api/v1/pods"))
            if "items" in pods:
                for item in pods["items"]:
                    if "spec" in item and "containers" in item["spec"]:
                        if "status" in item and "phase" in item["status"] and item["status"]["phase"] == "Running":
                            for container in item["spec"]["containers"]:
                                if "resources" in container and "requests" in container["resources"] and "nvidia.com/gpu" in container["resources"]["requests"]:
                                    used_gpus += int(container["resources"]["requests"]["nvidia.com/gpu"])
            vl = collectd.Values(type='gauge')
            vl.plugin = 'gpu'
            vl.plugin_instance = "usedgpu"
            vl.dispatch(values=[float(used_gpus)])
        except:
            exc_type, exc_value, exc_traceback = sys.exc_info()
            print "*** print_tb:"
            traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
            print "*** print_exception:"
            traceback.print_exception(exc_type, exc_value, exc_traceback,
                                      limit=2, file=sys.stdout)
            print "*** print_exc:"
            traceback.print_exc()

        try:
            total_gpus = 0
            nodes = json.loads( curl_get(os.environ['K8SAPI']+"/api/v1/nodes"))
            if "items" in nodes:
                for item in nodes["items"]:
                    if "status" in item and "capacity" in item["status"] and "nvidia.com/gpu" in item["status"]["capacity"]:
                        total_gpus += int(item["status"]["capacity"]["nvidia.com/gpu"])
            vl = collectd.Values(type='gauge')
            vl.plugin = 'gpu'
            vl.plugin_instance = "totalgpu"
            vl.dispatch(values=[float(total_gpus)])

        except:
            exc_type, exc_value, exc_traceback = sys.exc_info()
            print "*** print_tb:"
            traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
            print "*** print_exception:"
            traceback.print_exception(exc_type, exc_value, exc_traceback,
                                      limit=2, file=sys.stdout)
            print "*** print_exc:"
            traceback.print_exc()

# the major difference between read2 and read is 
# read2 uses the kubectl client to get cluster information 
# instead of sending http request to api server  
def read2(data=None):
        
    vl = collectd.Values(type='gauge')
    vl.plugin = 'kubernetes'

    try:
        rsset = json.loads(local_ssh_exec_cmd_with_output("kubectl get replicasets --all-namespaces -o json"))
        if "items" in rsset:
            for rs in rsset["items"]:
                if "metadata" in rs and "name" in rs["metadata"] and "status" in rs:
                        vl.plugin_instance = rs["metadata"]["name"]

                        if "availableReplicas" in rs["status"]:
                            numberAvailable = float(rs["status"]["availableReplicas"])
                        else:
                            numberAvailable = 0

                        if "replicas" in rs["status"]:
                            desiredNumber = float(rs["status"]["replicas"])
                        else:
                            desiredNumber = 0

                        if "readyReplicas" in rs["status"]:
                            readyNumber = float(rs["status"]["readyReplicas"])
                        else:
                            readyNumber = 0

                        collectd.info('kubernetes plugin: replicaset "%s" with values: %f %f %f' % (rs["metadata"]["name"],desiredNumber,numberAvailable,readyNumber))
                        if desiredNumber > 0 and desiredNumber == readyNumber and desiredNumber == numberAvailable:
                            res = 0
                        else:
                            res = 1

                        vl.dispatch(values=[float(res)])
                else:
                    pass
        else:
            ## no items
            pass

        rsset = json.loads(local_ssh_exec_cmd_with_output("kubectl get ReplicationController --all-namespaces -o json"))
        if "items" in rsset:
            for rs in rsset["items"]:
                if "metadata" in rs and "name" in rs["metadata"] and "status" in rs:
                        vl.plugin_instance = rs["metadata"]["name"]

                        if "availableReplicas" in rs["status"]:
                            numberAvailable = float(rs["status"]["availableReplicas"])
                        else:
                            numberAvailable = 0

                        if "replicas" in rs["status"]:
                            desiredNumber = float(rs["status"]["replicas"])
                        else:
                            desiredNumber = 0

                        if "readyReplicas" in rs["status"]:
                            readyNumber = float(rs["status"]["readyReplicas"])
                        else:
                            readyNumber = 0

                        collectd.info('kubernetes plugin: ReplicationController "%s" with values: %f %f %f' % (rs["metadata"]["name"],desiredNumber,numberAvailable,readyNumber))
                    
                        if desiredNumber > 0 and desiredNumber == readyNumber and desiredNumber == numberAvailable:
                            res = 0
                        else:
                            res = 1
                        vl.dispatch(values=[float(res)])
                else:
                    pass
        else:
            ## no items
            pass

        dpset = json.loads(local_ssh_exec_cmd_with_output("kubectl get daemonsets --all-namespaces -o json"))
        if "items" in dpset:
            for dp in dpset["items"]:
                if "metadata" in dp and "name" in dp["metadata"] and "status" in dp:
                        vl.plugin_instance = dp["metadata"]["name"]
                        if "numberAvailable" in dp["status"]:
                            numberAvailable = float(dp["status"]["numberAvailable"])
                        else:
                            numberAvailable = 0

                        if "desiredNumberScheduled" in dp["status"]:
                            desiredNumber = float(dp["status"]["desiredNumberScheduled"])
                        else:
                            desiredNumber = 0

                        if "numberReady" in dp["status"]:
                            readyNumber = float(dp["status"]["numberReady"])
                        else:
                            readyNumber = 0

                        collectd.info('kubernetes plugin: deployment "%s" with values: %f %f %f' % (dp["metadata"]["name"],desiredNumber,numberAvailable,readyNumber))
                    
                        if desiredNumber > 0 and desiredNumber == readyNumber and desiredNumber == numberAvailable:
                            res = 0
                        else:
                            res = 1
                        vl.dispatch(values=[float(res)])
                else:
                    pass
        else:
            ## no items
            pass

    except:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        print "*** print_tb:"
        traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
        print "*** print_exception:"
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                    limit=2, file=sys.stdout)
        print "*** print_exc:"
        traceback.print_exc()

    NVIDIA_GPU_RESOURCE_FLAG = "nvidia.com/gpu"
    HUAWEI_NPU_RESOURCE_FLAG = "npu.huawei.com/NPU"

    try:
        used_gpus = 0
        used_npus = 0
        pods = json.loads(local_ssh_exec_cmd_with_output("kubectl get pods --all-namespaces -o json"))

        if "items" in pods:
            for item in pods["items"]:

                if "spec" in item and "containers" in item["spec"]:

                    if "status" in item and "phase" in item["status"] and item["status"]["phase"] == "Running":

                        for container in item["spec"]["containers"]:

                            if "resources" in container and "requests" in container["resources"] and \
                                NVIDIA_GPU_RESOURCE_FLAG in container["resources"]["requests"]:
                                used_gpus += int(container["resources"]["requests"][NVIDIA_GPU_RESOURCE_FLAG])
                            else:
                                pass

                            if "resources" in container and "requests" in container["resources"] and \
                                HUAWEI_NPU_RESOURCE_FLAG in container["resources"]["requests"]:
                                used_npus += int(container["resources"]["requests"][HUAWEI_NPU_RESOURCE_FLAG])
                            else:
                                pass

                    else:
                        pass
                else:
                    pass
        else:
            pass
        
        vl = collectd.Values(type='gauge')
        vl.plugin = 'gpu'
        vl.plugin_instance = "usedgpu"
        vl.dispatch(values=[float(used_gpus)])

        v2 = collectd.Values(type='gauge')
        v2.plugin = 'npu'
        v2.plugin_instance = "usednpu"
        v2.dispatch(values=[float(used_npus)])

    except:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        print "*** print_tb:"
        traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
        print "*** print_exception:"
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                    limit=2, file=sys.stdout)
        print "*** print_exc:"
        traceback.print_exc()

    try:
        total_gpus = 0
        total_npus = 0
        nodes = json.loads(local_ssh_exec_cmd_with_output("kubectl get nodes -o json"))

        if "items" in nodes:
            for item in nodes["items"]:

                if "status" in item and "capacity" in item["status"] and \
                    NVIDIA_GPU_RESOURCE_FLAG in item["status"]["capacity"]:
                    total_gpus += int(item["status"]["capacity"][NVIDIA_GPU_RESOURCE_FLAG])
                else:
                    pass

                if "status" in item and "capacity" in item["status"] and \
                    HUAWEI_NPU_RESOURCE_FLAG in item["status"]["capacity"]:
                    total_npus += int(item["status"]["capacity"][HUAWEI_NPU_RESOURCE_FLAG])
                else:
                    pass
        else:
            pass

        vl = collectd.Values(type='gauge')
        vl.plugin = 'gpu'
        vl.plugin_instance = "totalgpu"
        vl.dispatch(values=[float(total_gpus)])

        v2 = collectd.Values(type='gauge')
        v2.plugin = 'npu'
        v2.plugin_instance = "totalnpu"
        v2.dispatch(values=[float(total_npus)])

    except:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        print "*** print_tb:"
        traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
        print "*** print_exception:"
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                    limit=2, file=sys.stdout)
        print "*** print_exc:"
        traceback.print_exc()

    return

collectd.register_config(configure)
collectd.register_read(read2)
