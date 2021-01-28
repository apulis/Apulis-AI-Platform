import yaml
import os
from Queue import Queue
import threading

try:
    f = open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "config.yaml"))
    config = yaml.load(f)
except Exception:
    config = {}

def GetWorkPath(workpath):
    workPath = "work/"+workpath
    return workPath

def GetStoragePath(jobpath, workpath, datapath):
    jobPath = "work/"+jobpath
    workPath = "work/"+workpath
    dataPath = "storage/"+datapath
    return jobPath,workPath,dataPath


class ResourceLimit(object):
    CPU="cpu"
    MEM="memory"

# None means no limit
# resource_type must be either one of ResourceLimit.CPU 
# or ResourceLimit.MEM
def GetResourceLimit(device_type, resource_type):

    if "resource_limit" in config and device_type in config["resource_limit"]:

        limits = config["resource_limit"][device_type]
        
        if resource_type == ResourceLimit.CPU and ResourceLimit.CPU in limits:
            return limits[ResourceLimit.CPU]

        elif resource_type == ResourceLimit.MEM and ResourceLimit.MEM in limits:
            return limits[ResourceLimit.MEM]

        else:
            pass
    else:
        # not configed
        pass

    return None


global global_vars
global_vars={}
global_vars["sql_connections"] = Queue()
global_vars["sql_connection_num"] = 0
global_vars["sql_lock"] = threading.Lock()
