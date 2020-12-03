import json
import os
import time
import argparse
import uuid
import subprocess
import sys
import random
import string
import collections
import copy

from jobs_tensorboard import GenTensorboardMeta

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../storage"))

import yaml
from jinja2 import Environment, FileSystemLoader, Template
from config import config
from DataHandler import DataHandler,DataManager
import base64
import re
import requests

from config import global_vars
from authorization import ResourceType, Permission, AuthorizationManager, IdentityManager, ACLManager
import authorization
import EndpointUtils
from cache import CacheManager
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../ClusterManager"))
from ResourceInfo import ResourceInfo
import inference
import quota

import copy
import logging
from cachetools import cached, TTLCache
from threading import Lock


DEFAULT_JOB_PRIORITY = 100
USER_JOB_PRIORITY_RANGE = (100, 200)
ADMIN_JOB_PRIORITY_RANGE = (1, 1000)


logger = logging.getLogger(__name__)

pendingStatus = "running,queued,scheduling,unapproved,pausing,paused"
DEFAULT_EXPIRATION = 24 * 30 * 60
vc_cache = TTLCache(maxsize=10240, ttl=DEFAULT_EXPIRATION)
vc_cache_lock = Lock()

def adjust_job_priority(priority, permission):
    priority_range = (DEFAULT_JOB_PRIORITY, DEFAULT_JOB_PRIORITY)
    if permission == Permission.User:
        priority_range = USER_JOB_PRIORITY_RANGE
    elif permission == Permission.Admin:
        priority_range = ADMIN_JOB_PRIORITY_RANGE

    if priority > priority_range[1]:
        priority = priority_range[1]
    elif priority < priority_range[0]:
        priority = priority_range[0]

    return priority


def LoadJobParams(jobParamsJsonStr):
    return json.loads(jobParamsJsonStr)


def ToBool(value):
    if isinstance(value, basestring):
        value = str(value)
        if str.isdigit(value):
            ret = int(value)
            if ret == 0:
                return False
            else:
                return True
        else:
            if value.upper() == 'TRUE':
                return True
            elif value.upper() == 'FALSE':
                return False
            else:
                raise ValueError
    elif isinstance(value, int):
        if value == 0:
            return False
        else:
            return True
    else:
        return value


def SubmitJob(jobParamsJsonStr):

    ret = {}
    jobParams = LoadJobParams(jobParamsJsonStr)

    if "jobName" not in jobParams or len(jobParams["jobName"].strip()) == 0:
        ret["error"] = "ERROR: Job name cannot be empty"
        return ret

    if "vcName" not in jobParams or len(jobParams["vcName"].strip()) == 0:
        ret["error"] = "ERROR: VC name cannot be empty"
        return ret

    if "userId" not in jobParams or len(str(jobParams["userId"]).strip()) == 0:
        jobParams["userId"] = GetUser(jobParams["userName"])["uid"]

    if "preemptionAllowed" not in jobParams:
        jobParams["preemptionAllowed"] = False
    else:
        jobParams["preemptionAllowed"] = ToBool(jobParams["preemptionAllowed"])

    if "preemptionAllowed" not in jobParams:
        jobParams["preemptionAllowed"] = False
    else:
        jobParams["preemptionAllowed"] = ToBool(jobParams["preemptionAllowed"])

    uniqId = str(uuid.uuid4())
    if "jobId" not in jobParams or jobParams["jobId"] == "":
        #jobParams["jobId"] = jobParams["jobName"] + "-" + str(uuid.uuid4())
        #jobParams["jobId"] = jobParams["jobName"] + "-" + str(time.time())
        jobParams["jobId"] = uniqId
    #jobParams["jobId"] = jobParams["jobId"].replace("_","-").replace(".","-")

    if "resourcegpu" not in jobParams:
        jobParams["resourcegpu"] = 0
    else:
        pass

    if isinstance(jobParams["resourcegpu"], basestring):
        if len(jobParams["resourcegpu"].strip()) == 0:
            jobParams["resourcegpu"] = 0
        else:
            jobParams["resourcegpu"] = int(jobParams["resourcegpu"])

    if "familyToken" not in jobParams or jobParams["familyToken"].isspace():
        jobParams["familyToken"] = uniqId
    if "isParent" not in jobParams:
        jobParams["isParent"] = 1

    userName = getAlias(jobParams["userName"])
        
    # return if there is not enough devices
    #if "gpuType" in jobParams:
    #    valid, msg = ValidateDeviceRequest(jobParams["gpuType"], jobParams["resourcegpu"], jobParams["vcName"].strip())
    #    if valid is False:
    #        ret["error"] = msg
    #        return
    #else:
    #    logger.info("gpuType not set!")
    #    pass

    if not AuthorizationManager.HasAccess(jobParams["userName"], ResourceType.VC, jobParams["vcName"].strip(), Permission.User):
        ret["error"] = "Access Denied!"
        return ret

    if "cmd" not in jobParams:
        jobParams["cmd"] = ""

    if "jobPath" in jobParams and len(jobParams["jobPath"].strip()) > 0:
        jobPath = jobParams["jobPath"]
        if ".." in jobParams["jobPath"]:
            ret["error"] = "ERROR: '..' cannot be used in job directory"
            return ret

        if "\\." in jobParams["jobPath"]:
            ret["error"] = "ERROR: invalided job directory"
            return ret

        if jobParams["jobPath"].startswith("/") or jobParams["jobPath"].startswith("\\"):
            ret["error"] = "ERROR: job directory should not start with '/' or '\\' "
            return ret

        if not jobParams["jobPath"].startswith(userName):
            if jobParams["jobPath"] == "./":
                jobParams["jobPath"] = os.path.join(userName,"jobs/"+time.strftime("%y%m%d")+"/"+jobParams["jobId"], jobParams["jobPath"])
            else:
                jobParams["jobPath"] = os.path.join(userName,jobParams["jobPath"])


    else:
        jobPath = userName+"/"+ "jobs/"+time.strftime("%y%m%d")+"/"+jobParams["jobId"]
        jobParams["jobPath"] = jobPath

    if "workPath" not in jobParams or len(jobParams["workPath"].strip()) == 0:
       jobParams["workPath"] = "."

    if ".." in jobParams["workPath"]:
        ret["error"] = "ERROR: '..' cannot be used in work directory"
        return ret

    if "\\." in jobParams["workPath"]:
        ret["error"] = "ERROR: invalided work directory"
        return ret

    if jobParams["workPath"].startswith("/") or jobParams["workPath"].startswith("\\"):
        ret["error"] = "ERROR: work directory should not start with '/' or '\\' "
        return ret

    if not jobParams["workPath"].startswith(userName):
        jobParams["workPath"] = os.path.join(userName,jobParams["workPath"])

    if "dataPath" not in jobParams or len(jobParams["dataPath"].strip()) == 0:
        jobParams["dataPath"] = "."

    if ".." in jobParams["dataPath"]:
        ret["error"] = "ERROR: '..' cannot be used in data directory"
        return ret

    if "\\." in jobParams["dataPath"]:
        ret["error"] = "ERROR: invalided data directory"
        return ret

    if jobParams["dataPath"][0] == "/" or jobParams["dataPath"][0] == "\\":
        ret["error"] = "ERROR: data directory should not start with '/' or '\\' "
        return ret

    jobParams["dataPath"] = jobParams["dataPath"].replace("\\","/")
    jobParams["workPath"] = jobParams["workPath"].replace("\\","/")
    jobParams["jobPath"] = jobParams["jobPath"].replace("\\","/")

    jobParams["dataPath"] = os.path.realpath(os.path.join("/",jobParams["dataPath"]))[1:]
    jobParams["workPath"] = os.path.realpath(os.path.join("/",jobParams["workPath"]))[1:]
    jobParams["jobPath"] = os.path.realpath(os.path.join("/",jobParams["jobPath"]))[1:]

    dataHandler = DataHandler()
    if "logDir" in jobParams and len(jobParams["logDir"].strip()) > 0:
        tensorboardParams = jobParams.copy()

        # overwrite for distributed job
        if tensorboardParams["jobtrainingtype"] == "PSDistJob":
            tensorboardParams["jobtrainingtype"] = "RegularJob"
            match = re.match('(.*)(/.*)', tensorboardParams["logDir"])
            if not match is None:
                newDir = match.group(1) + "/worker0" + match.group(2)
                prefix = match.group(1)
                match2 = re.match('.*/worker0', prefix)
                if match2 is None:
                    tensorboardParams["logDir"] = newDir
            #match = re.match('(.*--logdir\s+.*)(/.*--.*)', tensorboardParams["cmd"])
            #if not match is None:
            #    tensorboardParams["cmd"] = match.group(1) + "/worker0" + match.group(2)

        tensorboardParams["jobId"] = uniqId
        tensorboardParams["jobName"] = "tensorboard-"+jobParams["jobName"]
        tensorboardParams["jobPath"] = jobPath
        tensorboardParams["jobType"] = "visualization"
        tensorboardParams["cmd"] = "tensorboard --logdir " + tensorboardParams["logDir"] + " --host 0.0.0.0"
        tensorboardParams["image"] = jobParams["image"]
        tensorboardParams["resourcegpu"] = 0

        tensorboardParams["interactivePort"] = "6006"


        if "error" not in ret:
            if not dataHandler.AddJob(tensorboardParams):
                ret["error"] = "Cannot schedule tensorboard job."

    if "error" not in ret:
        if dataHandler.AddJob(jobParams):
            ret["jobId"] = jobParams["jobId"]
            if "jobPriority" in jobParams:
                priority = DEFAULT_JOB_PRIORITY
                try:
                    priority = int(jobParams["jobPriority"])
                except Exception as e:
                    pass

                permission = Permission.User
                if AuthorizationManager.HasAccess(jobParams["userName"], ResourceType.VC, jobParams["vcName"].strip(), Permission.Admin):
                    permission = Permission.Admin

                priority = adjust_job_priority(priority, permission)

                job_priorities = {jobParams["jobId"]: priority}
                dataHandler.update_job_priority(job_priorities)
        else:
            ret["error"] = "Cannot schedule job. Cannot add job into database."

    dataHandler.Close()
    return ret

def PostInferenceJob(jobParamsJsonStr):
    ret = {}

    jobParams = LoadJobParams(jobParamsJsonStr)

    if "jobName" not in jobParams or len(jobParams["jobName"].strip()) == 0:
        ret["error"] = "ERROR: Job name cannot be empty"
        return ret
    if "vcName" not in jobParams or len(jobParams["vcName"].strip()) == 0:
        ret["error"] = "ERROR: VC name cannot be empty"
        return ret
    if "userId" not in jobParams or len(str(jobParams["userId"]).strip()) == 0:
        jobParams["userId"] = GetUser(jobParams["userName"])["uid"]

    uniqId = str(uuid.uuid4())
    if "jobId" not in jobParams or jobParams["jobId"] == "":
        jobParams["jobId"] = uniqId

    if "resourcegpu" not in jobParams:
        jobParams["resourcegpu"] = 0
    if "device" in jobParams and jobParams["device"] == "CPU":
        jobParams["resourcegpu"] = 0

    # default value
    jobParams["jobtrainingtype"] = "InferenceJob"
    jobParams["jobType"] = "InferenceJob"
    jobParams["preemptionAllowed"] = False
    jobParams["containerUserId"] = 0
    jobParams["isPrivileged"] = False
    jobParams["hostNetwork"] = False
    jobParams["gpuType"] = None if "gpuType" not in jobParams else jobParams["gpuType"]

    if isinstance(jobParams["resourcegpu"], basestring):
        if len(jobParams["resourcegpu"].strip()) == 0:
            jobParams["resourcegpu"] = 0
        else:
            jobParams["resourcegpu"] = int(jobParams["resourcegpu"])

    if "familyToken" not in jobParams or jobParams["familyToken"].isspace():
        jobParams["familyToken"] = uniqId
    if "isParent" not in jobParams:
        jobParams["isParent"] = 1

    userName = getAlias(jobParams["userName"])

    if "cmd" not in jobParams:
        jobParams["cmd"] = ""

    if "jobPath" in jobParams and len(jobParams["jobPath"].strip()) > 0:
        jobPath = jobParams["jobPath"]
        if ".." in jobParams["jobPath"]:
            ret["error"] = "ERROR: '..' cannot be used in job directory"
            return ret

        if "\\." in jobParams["jobPath"]:
            ret["error"] = "ERROR: invalided job directory"
            return ret

        if jobParams["jobPath"].startswith("/") or jobParams["jobPath"].startswith("\\"):
            ret["error"] = "ERROR: job directory should not start with '/' or '\\' "
            return ret

        if not jobParams["jobPath"].startswith(userName):
            jobParams["jobPath"] = os.path.join(userName,jobParams["jobPath"])

    else:
        jobPath = userName+"/"+ "jobs/"+time.strftime("%y%m%d")+"/"+jobParams["jobId"]
        jobParams["jobPath"] = jobPath

    if "workPath" not in jobParams or len(jobParams["workPath"].strip()) == 0:
       jobParams["workPath"] = "."

    if ".." in jobParams["workPath"]:
        ret["error"] = "ERROR: '..' cannot be used in work directory"
        return ret

    if "\\." in jobParams["workPath"]:
        ret["error"] = "ERROR: invalided work directory"
        return ret

    if jobParams["workPath"].startswith("/") or jobParams["workPath"].startswith("\\"):
        ret["error"] = "ERROR: work directory should not start with '/' or '\\' "
        return ret

    if not jobParams["workPath"].startswith(userName):
        jobParams["workPath"] = os.path.join(userName,jobParams["workPath"])

    if "dataPath" not in jobParams or len(jobParams["dataPath"].strip()) == 0:
        jobParams["dataPath"] = "."

    if ".." in jobParams["dataPath"]:
        ret["error"] = "ERROR: '..' cannot be used in data directory"
        return ret

    if "\\." in jobParams["dataPath"]:
        ret["error"] = "ERROR: invalided data directory"
        return ret

    if jobParams["dataPath"][0] == "/" or jobParams["dataPath"][0] == "\\":
        ret["error"] = "ERROR: data directory should not start with '/' or '\\' "
        return ret

    jobParams["dataPath"] = jobParams["dataPath"].replace("\\","/")
    jobParams["workPath"] = jobParams["workPath"].replace("\\","/")
    jobParams["jobPath"] = jobParams["jobPath"].replace("\\","/")
    jobParams["dataPath"] = os.path.realpath(os.path.join("/",jobParams["dataPath"]))[1:]
    jobParams["workPath"] = os.path.realpath(os.path.join("/",jobParams["workPath"]))[1:]
    jobParams["jobPath"] = os.path.realpath(os.path.join("/",jobParams["jobPath"]))[1:]

    dataHandler = DataHandler()

    if "error" not in ret:
        if dataHandler.AddJob(jobParams):
            ret["jobId"] = jobParams["jobId"]
            if "jobPriority" in jobParams:
                priority = DEFAULT_JOB_PRIORITY
                try:
                    priority = int(jobParams["jobPriority"])
                except Exception as e:
                    pass

                permission = Permission.User
                if AuthorizationManager.HasAccess(jobParams["userName"], ResourceType.VC, jobParams["vcName"].strip(), Permission.Admin):
                    permission = Permission.Admin

                priority = adjust_job_priority(priority, permission)

                job_priorities = {jobParams["jobId"]: priority}
                dataHandler.update_job_priority(job_priorities)
            if "jobtrainingtype" in jobParams and jobParams["jobtrainingtype"] == "InferenceJob":
                dataHandler.AddInferenceJob(jobParams)
        else:
            ret["error"] = "Cannot schedule job. Cannot add job into database."

    dataHandler.Close()
    return ret

def GetModelConversionTypes():
    return [
        "arm64-caffe-Ascend310",
        "arm64-tensorflow-Ascend310",
        "x8664-caffe-Ascend310",
        "x8664-tensorflow-Ascend310"
    ]

def BuildModelConversionArgs(conversionArgs):
    arg_str = ""
    string_type_args = [
        "input_shape", "output_type", "dynamic_batch_size", "dynamic_image_size"
    ]
    for key, value in conversionArgs.items():
        if value is not None or value != "":
            if key in string_type_args:
                value = '"' + value + '"'
            else:
                value = "".join(value.split())
            arg_str = arg_str + " --" + key + "=" + value
    return arg_str

def PostModelConversionJob(jobParamsJsonStr):
    ret = {}

    jobParams = LoadJobParams(jobParamsJsonStr)

    if "jobName" not in jobParams or len(jobParams["jobName"].strip()) == 0:
        ret["error"] = "ERROR: Job name cannot be empty"
        return ret
    if "vcName" not in jobParams or len(jobParams["vcName"].strip()) == 0:
        ret["error"] = "ERROR: VC name cannot be empty"
        return ret
    if "userId" not in jobParams or len(str(jobParams["userId"]).strip()) == 0:
        jobParams["userId"] = GetUser(jobParams["userName"])["uid"]

    uniqId = str(uuid.uuid4())
    if "jobId" not in jobParams or jobParams["jobId"] == "":
        jobParams["jobId"] = uniqId

    if "resourcegpu" not in jobParams:
        jobParams["resourcegpu"] = 0

    # default value
    jobParams["jobtrainingtype"] = "RegularJob"
    jobParams["jobType"] = "ModelConversionJob"
    jobParams["preemptionAllowed"] = False
    jobParams["containerUserId"] = 0
    jobParams["isPrivileged"] = False
    jobParams["hostNetwork"] = False

    # atc values
    if jobParams["conversionType"] in GetModelConversionTypes():
        input_path = jobParams["inputPath"] if "inputPath" in jobParams else ""
        output_path = jobParams["outputPath"] if "outputPath" in jobParams else ""
        if output_path.endswith('.om'):
            output_path = output_path[:-3]
        output_dir = '/'.join(output_path.split('/')[:-1])

        raw_cmd = "atc --model=%s --output=%s --soc_version=Ascend310" % (input_path, output_path)

        if "tensorflow" in jobParams["conversionType"]:
            raw_cmd = raw_cmd + " --framework=3"
        elif "caffe" in jobParams["conversionType"]:
            raw_cmd = raw_cmd + " --framework=0"

        if "conversionArgs" in jobParams:
            raw_cmd = raw_cmd + BuildModelConversionArgs(jobParams["conversionArgs"])

        jobParams["cmd"] = 'sudo bash -E -c "source /pod.env && %s && chmod 777 %s && chmod 777 %s"' % (raw_cmd, output_dir, output_path + ".om")

    else:
        ret["error"] = "ERROR: .. convert type " + jobParams["conversionType"] + " not supported"
        return ret

    # env
    if isinstance(jobParams["resourcegpu"], basestring):
        if len(jobParams["resourcegpu"].strip()) == 0:
            jobParams["resourcegpu"] = 0
        else:
            jobParams["resourcegpu"] = int(jobParams["resourcegpu"])

    if "familyToken" not in jobParams or jobParams["familyToken"].isspace():
        jobParams["familyToken"] = uniqId
    if "isParent" not in jobParams:
        jobParams["isParent"] = 1

    userName = getAlias(jobParams["userName"])


    if "jobPath" in jobParams and len(jobParams["jobPath"].strip()) > 0:
        jobPath = jobParams["jobPath"]
        if ".." in jobParams["jobPath"]:
            ret["error"] = "ERROR: '..' cannot be used in job directory"
            return ret

        if "\\." in jobParams["jobPath"]:
            ret["error"] = "ERROR: invalided job directory"
            return ret

        if jobParams["jobPath"].startswith("/") or jobParams["jobPath"].startswith("\\"):
            ret["error"] = "ERROR: job directory should not start with '/' or '\\' "
            return ret

        if not jobParams["jobPath"].startswith(userName):
            jobParams["jobPath"] = os.path.join(userName,jobParams["jobPath"])

    else:
        jobPath = userName+"/"+ "jobs/"+time.strftime("%y%m%d")+"/"+jobParams["jobId"]
        jobParams["jobPath"] = jobPath

    if "workPath" not in jobParams or len(jobParams["workPath"].strip()) == 0:
       jobParams["workPath"] = "."

    if ".." in jobParams["workPath"]:
        ret["error"] = "ERROR: '..' cannot be used in work directory"
        return ret

    if "\\." in jobParams["workPath"]:
        ret["error"] = "ERROR: invalided work directory"
        return ret

    if jobParams["workPath"].startswith("/") or jobParams["workPath"].startswith("\\"):
        ret["error"] = "ERROR: work directory should not start with '/' or '\\' "
        return ret

    if not jobParams["workPath"].startswith(userName):
        jobParams["workPath"] = os.path.join(userName,jobParams["workPath"])

    if "dataPath" not in jobParams or len(jobParams["dataPath"].strip()) == 0:
        jobParams["dataPath"] = "."

    if ".." in jobParams["dataPath"]:
        ret["error"] = "ERROR: '..' cannot be used in data directory"
        return ret

    if "\\." in jobParams["dataPath"]:
        ret["error"] = "ERROR: invalided data directory"
        return ret

    if jobParams["dataPath"][0] == "/" or jobParams["dataPath"][0] == "\\":
        ret["error"] = "ERROR: data directory should not start with '/' or '\\' "
        return ret

    jobParams["dataPath"] = jobParams["dataPath"].replace("\\","/")
    jobParams["workPath"] = jobParams["workPath"].replace("\\","/")
    jobParams["jobPath"] = jobParams["jobPath"].replace("\\","/")
    jobParams["dataPath"] = os.path.realpath(os.path.join("/",jobParams["dataPath"]))[1:]
    jobParams["workPath"] = os.path.realpath(os.path.join("/",jobParams["workPath"]))[1:]
    jobParams["jobPath"] = os.path.realpath(os.path.join("/",jobParams["jobPath"]))[1:]

    dataHandler = DataHandler()

    if "error" not in ret:
        if dataHandler.AddJob(jobParams):
            ret["jobId"] = jobParams["jobId"]
            if "jobPriority" in jobParams:
                priority = DEFAULT_JOB_PRIORITY
                try:
                    priority = int(jobParams["jobPriority"])
                except Exception as e:
                    pass

                permission = Permission.User
                if AuthorizationManager.HasAccess(jobParams["userName"], ResourceType.VC, jobParams["vcName"].strip(), Permission.Admin):
                    permission = Permission.Admin

                priority = adjust_job_priority(priority, permission)

                job_priorities = {jobParams["jobId"]: priority}
                dataHandler.update_job_priority(job_priorities)
            if "jobType" in jobParams and jobParams["jobType"] == "ModelConversionJob":
                dataHandler.AddModelConversionJob(jobParams)
        else:
            ret["error"] = "Cannot schedule job. Cannot add job into database."

    dataHandler.Close()
    return ret

def get_type_and_num(resources,deviceString):
    ret = {}
    for deviceType, details in resources.items():
        if details["deviceStr"] == deviceString:
            ret[deviceType]={"capacity":details["capacity"]}
    return ret

def GetAllSupportInference():
    ret = collections.defaultdict(lambda :collections.defaultdict(lambda :{}))
    gpuStrList = {"npu":"npu.huawei.com/NPU","gpu":"nvidia.com/gpu"}
    try:
        dataHandler = DataHandler()
        resources = dataHandler.GetAllDevice()
        if "inference" in config:
            for framework, items in config["inference"].items():
                versionlist = items['allowedImageVersions']
                tmp=collections.defaultdict(lambda :[])
                if versionlist:
                    for one in versionlist:
                        if "-" in one:
                            version,suffix = one.split("-")
                            if suffix=="arm64":
                                suffix = "cpu"
                                details = get_type_and_num(resources,gpuStrList["npu"])
                            else:
                                details = get_type_and_num(resources, gpuStrList[suffix])

                        else:
                            version,suffix = one,"amd64"
                            suffix = "cpu"
                            details = get_type_and_num(resources, gpuStrList["gpu"])

                        tmp[version].append({"image":"","device":suffix,"details":details})

                for current_version,item_list in tmp.items():
                    for one in item_list:
                        ret[framework][current_version].update(one["details"])

                # if "custom" in config["inference"]:
                #     ret["custom"]["cpu"].update(get_type_and_num(resources,gpuStrList["npu"]))
                #     ret["custom"]["cpu"].update(get_type_and_num(resources,gpuStrList["gpu"]))
                #     ret["custom"]["gpu"].update(get_type_and_num(resources,gpuStrList["gpu"]))
                #     ret["custom"]["npu"].update(get_type_and_num(resources,gpuStrList["npu"]))

    except Exception as e:
        logger.exception('Exception: %s', str(e))
    return ret

def GetJobList(userName, vcName, jobOwner, num=None, pageSize=None, pageNum=None, jobName=None ):
    try:
        dataHandler = DataHandler()
        jobs = []
        hasAccessOnAllJobs = False

        if AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.Collaborator):
            hasAccessOnAllJobs = True

        if jobOwner != "all" or not hasAccessOnAllJobs:
            jobs = jobs + GetUserPendingJobs(userName, vcName)
            jobs = jobs + dataHandler.GetJobList(userName,vcName,num, pageSize, pageNum, jobName, "running,queued,scheduling,unapproved,pausing,paused", ("<>","and"))
        else:
            jobs = GetUserPendingJobs(jobOwner, vcName)

        for job in jobs:
            job.pop('jobMeta', None)
        dataHandler.Close()
        return jobs
    except Exception as e:
        logger.error('Exception: %s', str(e))
        logger.warn("Fail to get job list for user %s, return empty list", userName)
        return []

def GetJobListV2(userName, vcName, jobOwner, num=None):
    jobs = {}
    dataHandler = None
    try:
        dataHandler = DataHandler()

        hasAccessOnAllJobs = False
        if jobOwner == "all":
            hasAccessOnAllJobs = AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.Collaborator)

        # if user needs to access all jobs, and has been authorized, he could get all pending jobs; otherwise, he could get his own jobs with all status
        if hasAccessOnAllJobs:
            jobs = dataHandler.GetJobListV2("all", vcName, num, pendingStatus, ("=","or"))
        else:
            jobs = dataHandler.GetJobListV2(userName, vcName, num)

    except Exception as e:
        logger.error('get job list V2 Exception: user: %s, ex: %s', userName, str(e))
    finally:
        if dataHandler is not None:
            dataHandler.Close()
    return jobs

def GetJobListV3(userName, vcName, jobOwner, jobType, jobStatus, pageNum, pageSize, searchWord, orderBy, order):

    jobs = {}
    dataHandler = None

    try:
        dataHandler = DataHandler()
        hasAccessOnAllJobs = False

        # if user needs to access all jobs, and has been authorized,
        # he could get all pending jobs; otherwise, he could get his
        # own jobs with all status
        jobs = dataHandler.GetJobListV3(userName, vcName, jobType, jobStatus, pageNum, pageSize, searchWord, orderBy, order)

    except Exception as e:
        logger.error('get job list V2 Exception: user: %s, ex: %s', userName, str(e))

    finally:
        if dataHandler is not None:
            dataHandler.Close()
        else:
            pass

    return jobs


def GetJobCount(vcName, jobType, jobStatus, searchWord):
    count = 0
    dataHandler = None

    try:
        dataHandler = DataHandler()
        count = dataHandler.GetJobCount(vcName, jobType, jobStatus, searchWord)
    except Exception as e:
        logger.error('get all job list Exception: ex: %s', str(e))
    finally:
        if dataHandler is not None:
            dataHandler.Close()
        else:
            pass

    return {"count" : count}

def GetAllJobList( vcName, jobType, jobStatus, pageNum, pageSize, searchWord, orderBy, order):
    jobs = {}
    dataHandler = None

    try:
        dataHandler = DataHandler()
        hasAccessOnAllJobs = False

        # if user needs to access all jobs, and has been authorized,
        # he could get all pending jobs; otherwise, he could get his
        # own jobs with all status
        jobs = dataHandler.GetAllJobList( vcName, jobType, jobStatus, pageNum, pageSize, searchWord, orderBy, order)

    except Exception as e:
        logger.error('get all job list Exception: ex: %s', str(e))

    finally:
        if dataHandler is not None:
            dataHandler.Close()
        else:
            pass

    return jobs

def GetVCPendingJobs(userName, vcName):
    ret = {}
    jobs = {}

    ret["code"] = 0
    ret["data"] = []
    dataHandler = None

    try:
        global pendingStatus
        dataHandler = DataHandler()
        jobs = dataHandler.GetUserJobs(userName, vcName, pendingStatus)

        ret["msg"] = "success!"
        ret["data"] = jobs

    except Exception as e:
        ret["code"] = -1
        ret["msg"] = "failed! err: %s" % (str(e))
        logger.error('get job list Exception: user: %s, ex: %s', userName, str(e))

    finally:
        if dataHandler is not None:
            dataHandler.Close()
        else:
            pass

    return ret

def ListInferenceJob(jobOwner,vcName,num,search=None,status=None,order=None,orderBy=None):
    jobs = {}
    dataHandler = None
    try:
        dataHandler = DataHandler()
        if jobOwner == "all":
            jobs = dataHandler.ListInferenceJob("all", vcName, num, status, ("=", "or"),jobName=search)
        else:
            jobs = dataHandler.ListInferenceJobV2(jobOwner, vcName, num,status,jobName=search,order=order,orderBy=orderBy)
    except Exception as e:
        logger.error('ListInferenceJob Exception: user: %s, ex: %s', jobOwner, str(e))
    finally:
        if dataHandler is not None:
            dataHandler.Close()
    return jobs

def ListModelConversionJob(jobOwner,vcName,pageNum=None, pageSize=None, name=None, type=None, order=None, orderBy=None, jobStatus=None, convStatus=None):
    jobs = {}
    dataHandler = None
    try:
        dataHandler = DataHandler()
        if jobOwner == "all":
            jobs = dataHandler.ListModelConversionJob("all", vcName, pendingStatus, ("=", "or"), pageNum=pageNum, pageSize=pageSize, name=name, type=type, order=order, orderBy=orderBy, status=jobStatus, convStatus=convStatus)
        else:
            jobs = dataHandler.ListModelConversionJob(jobOwner, vcName, pageNum=pageNum, pageSize=pageSize, name=name, type=type, order=order, orderBy=orderBy, status=jobStatus, convStatus=convStatus)
    except Exception as e:
        logger.error('ListInferenceJob Exception: user: %s, ex: %s', jobOwner, str(e))
    finally:
        if dataHandler is not None:
            dataHandler.Close()
    return jobs

def GetUserPendingJobs(userName, vcName):
    jobs = []
    allJobs = DataManager.GetAllPendingJobs(vcName)
    for job in allJobs:
        if userName == "all" or userName == job["userName"]:
            jobs.append(job)
    return jobs


def GetCommands(userName, jobId):
    commands = []
    dataHandler = DataHandler()
    job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName"])
    if job is not None:
        if job["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Collaborator):
            commands = dataHandler.GetCommands(jobId=jobId)
    dataHandler.Close()
    return commands


def KillJob(userName, jobId):
    ret = False
    dataHandler = DataHandler()
    job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName", "jobStatus", "isParent", "familyToken"])
    if job is not None and job["jobStatus"].lower() in pendingStatus.split(","):
        if job["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Admin):
            dataFields = {"jobStatus": "killing"}
            conditionFields = {"jobId": jobId}
            if job["isParent"] == 1:
                conditionFields = {"familyToken": job["familyToken"]}
            ret = dataHandler.UpdateJobTextFields(conditionFields, dataFields)
    dataHandler.Close()
    return ret

def DeleteJob(jobId):
    CanDeleteJobStatus = ["failed","error","unapproved","finished","killed","paused"]
    ret = False
    dataHandler = DataHandler()
    job = dataHandler.GetJobTextFields(jobId, ["jobStatus"])
    if job is not None and job["jobStatus"].lower() in CanDeleteJobStatus:
        ret = dataHandler.DeleteJob(jobId)
    dataHandler.Close()
    return ret

def InvalidateJobListCache(vcName):
    CacheManager.Invalidate("GetAllPendingJobs", vcName)
    DataManager.GetAllPendingJobs(vcName)


def AddCommand(userName, jobId,command):
    dataHandler = DataHandler()
    ret = False
    job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName"])
    if job is not None:
        if job["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Collaborator):
            ret = dataHandler.AddCommand(jobId,command)
    dataHandler.Close()
    return ret


def ApproveJob(userName, jobId):
    dataHandler = DataHandler()
    ret = False
    job = dataHandler.GetJobTextFields(jobId, ["vcName", "jobStatus"])
    if job is not None and job["jobStatus"] == "unapproved":
        if AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Admin):
            ret = dataHandler.UpdateJobTextField(jobId,"jobStatus","queued")
    dataHandler.Close()
    return ret


def ResumeJob(userName, jobId):
    dataHandler = DataHandler()
    ret = False
    job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName", "jobStatus"])
    if job is not None and job["jobStatus"] == "paused":
        if job["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Collaborator):
            ret = dataHandler.UpdateJobTextField(jobId, "jobStatus", "unapproved")
    dataHandler.Close()
    return ret


def PauseJob(userName, jobId):
    dataHandler = DataHandler()
    ret = False
    job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName", "jobStatus"])
    if job is not None and job["jobStatus"] in ["unapproved", "queued", "scheduling", "running"]:
        if job["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Admin):
            ret = dataHandler.UpdateJobTextField(jobId,"jobStatus","pausing")
    dataHandler.Close()
    return ret


def isBase64(s):
    try:
        if base64.b64encode(base64.b64decode(s)) == s:
            return True
    except Exception as e:
        pass
    return False


def GetJobDetail(userName, jobId):
    job = None
    dataHandler = DataHandler()
    jobs =  dataHandler.GetJob(jobId=jobId)
    if len(jobs) == 1:
        if jobs[0]["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, jobs[0]["vcName"], Permission.Collaborator):
            job = jobs[0]
            job["log"] = ""
            #jobParams = json.loads(base64.b64decode(job["jobMeta"]))
            #jobPath,workPath,dataPath = GetStoragePath(jobParams["jobPath"],jobParams["workPath"],jobParams["dataPath"])
            #localJobPath = os.path.join(config["storage-mount-path"],jobPath)
            #logPath = os.path.join(localJobPath,"joblog.txt")
            #print logPath
            #if os.path.isfile(logPath):
            #    with open(logPath, 'r') as f:
            #        log = f.read()
            #        job["log"] = log
            #    f.close()
            if "jobDescription" in job:
                job.pop("jobDescription",None)
            try:
                log = dataHandler.GetJobTextField(jobId,"jobLog")
                try:
                    if isBase64(log):
                        log = base64.b64decode(log)
                except Exception:
                    pass
                if log is not None:
                    job["log"] = log
            except:
                job["log"] = "fail-to-get-logs"
    dataHandler.Close()
    return job

def GetJobDetailV2(userName, jobId):
    job = {}
    dataHandler = None
    try:
        dataHandler = DataHandler()
        jobs = dataHandler.GetJobV2(jobId)
        if len(jobs) == 1:
            if jobs[0]["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, jobs[0]["vcName"], Permission.Collaborator):
                job = jobs[0]
    except Exception as e:
        logger.error("get job detail v2 exception for user: %s, jobId: %s, exception: %s", userName, jobId, str(e))
    finally:
        if dataHandler is not None:
            dataHandler.Close()
    return job

def GetInferenceJobDetail(userName, jobId):
    job = {}
    dataHandler = None
    try:
        dataHandler = DataHandler()
        jobs = dataHandler.GetInferenceJob(jobId)
        if len(jobs) == 1:
            if jobs[0]["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, jobs[0]["vcName"], Permission.Collaborator):
                job = jobs[0]
    except Exception as e:
        logger.error("get job detail v2 exception for user: %s, jobId: %s, exception: %s", userName, jobId, str(e))
    finally:
        if dataHandler is not None:
            dataHandler.Close()
    return job

def GetJobStatus(jobId):
    result = None
    dataHandler = DataHandler()
    result = dataHandler.GetJobTextFields(jobId, ["jobStatus", "jobTime", "errorMsg"])
    dataHandler.Close()
    return result

def GetJobLog(userName, jobId,page=1):
    dataHandler = DataHandler()
    jobs =  dataHandler.GetJob(jobId=jobId)
    if len(jobs) == 1:
        if jobs[0]["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, jobs[0]["vcName"], Permission.Collaborator):
            try:
                # log = dataHandler.GetJobTextField(jobId,"jobLog")
                jobParams = json.loads(base64.b64decode(jobs[0]["jobParams"]))
                jobPath = "work/"+jobParams["jobPath"]
                localJobPath = os.path.join(config["storage-mount-path"], jobPath)
                logPath = os.path.join(localJobPath, "logs")
                max_page = 1
                if not page:
                    page = 1
                page = int(page)
                if os.path.exists(os.path.join(logPath,"max_page")):
                    with open(os.path.join(logPath,"max_page"),"r") as f:
                        max_page = int(f.read())
                if max_page<page:
                    page = max_page
                if os.path.exists(os.path.join(logPath,"log-container-" +jobId + ".txt"+"."+str(page))):
                    with open(os.path.join(logPath,"log-container-" + jobId + ".txt"+"."+str(page)), "r") as f:
                        log = f.read()
                    if log is not None:
                        return {
                            "log": log,
                            "cursor": None,
                            "max_page":max_page
                        }
            except Exception as e:
                logger.exception(e)
                pass
    return {
        "log": "",
        "cursor": None,
        "max_page":0
    }

def GetClusterStatus():
    cluster_status,last_update_time =  DataManager.GetClusterStatus()
    return cluster_status,last_update_time

def ListUser():
    dataHandler = DataHandler()
    ret = dataHandler.ListUser()
    dataHandler.Close()
    return ret

def updateUserPerm(identityName,isAdmin,isAuthorized):
    dataHandler = DataHandler()
    ret = dataHandler.UpdateIdentityInfoPerm(identityName,isAdmin,isAuthorized)
    dataHandler.Close()
    return ret

def Infer(jobId,image,signature_name):
    dataHandler = DataHandler()
    jobs = dataHandler.GetInferenceJob(jobId)
    if not signature_name:
        signature_name = "predict"
    if len(jobs)>0:
        job = jobs[0]
        if "inference-url" in job:
            inference_url = job["inference-url"]
            # ret = inference.object_classifier_infer(inference_url,image,signature_name)
            ret = inference.object_detaction_infer2(inference_url,image,signature_name,job["jobParams"])
        else:
            ret = "job not running"
    else:
        ret = "can't get job for jobid: " + jobId
    return ret


def AddUser(username,uid,gid,groups):
    ret = None
    needToUpdateDB = False

    if uid == authorization.INVALID_ID:
        info = IdentityManager.GetIdentityInfoFromDB(username)
        if info["uid"] == authorization.INVALID_ID:
            needToUpdateDB = True
            info = IdentityManager.GetIdentityInfoFromAD(username)
        uid = info["uid"]
        gid = info["gid"]
        groups = info["groups"]
    else:
        needToUpdateDB = True

    if needToUpdateDB:
        ret = IdentityManager.UpdateIdentityInfo(username, uid, gid, groups)
        ret = ret & ACLManager.UpdateAclIdentityId(username,uid)
    return ret

def SignUp(openId, group, nickName, userName, password, isAdmin = False, isAuthorized = False):
    ret = {}
    try:
        dataHandler = DataHandler()
        lst = dataHandler.GetAccountByOpenId(openId, group)

        # Register
        if len(lst) == 0:
            if nickName is None or len(nickName) < 1:
                ret["error"] = "NickName is too short"
                return  ret
            if userName is None or len(userName) < 3:
                ret["error"] = "UserName is too short (minimum is 3 characters)"
                return  ret
            if password is None or len(password) < 6:
                ret["error"] = "Password is too short (minimum is 6 characters)"
                return  ret
            # Check UserName available
            if len(dataHandler.GetAccountByUserName(userName)) > 0:
                ret["error"] = ('UserName %s is not available!') % userName
                return  ret
            if password is None or len(password) < 6:
                password = ''.join(random.sample(string.ascii_letters + string.digits, 8))
            dataHandler.UpdateAccountInfo(openId, group, nickName, userName, password, isAdmin, isAuthorized)
            lst = dataHandler.GetAccountByOpenId(openId, group)

        if len(lst) > 0:
            accountInfo = lst[0]
            identityInfo = IdentityManager.GetIdentityInfoFromDB(userName)
            if identityInfo["uid"] == authorization.INVALID_ID:
                GROUP_DICT = {
                    "Microsoft": 3001,
                    "Zhejianglab": 3002,
                    "Wechat": 3003,
                    "DingTalk": 3004,
                    "Account": 3005
                }
                if group in GROUP_DICT:
                    gid = GROUP_DICT[group]
                else:
                    gid = 3999
                    # groups = ['Other']
                groups = [gid]
                dataHandler.UpdateIdentityInfo(userName, accountInfo["uid"], gid, groups)

                # Update Ace
                permission = Permission.Admin if isAdmin else (Permission.User if isAuthorized else Permission.Unauthorized)
                resourceAclPath = AuthorizationManager.GetResourceAclPath("", ResourceType.Cluster)
                # only for account signup
                if group == "Account":
                    ACLManager.UpdateAce(userName, resourceAclPath, permission, 0)

        dataHandler.Close()
    except Exception as e:
        logger.error('Exception: %s', str(e))
        ret["error"] = 'Exception: %s', str(e)
    return ret


def GetAccountByOpenId(openId, group):
    ret = None
    try:
        dataHandler = DataHandler()
        lst = dataHandler.GetAccountByOpenId(openId, group)
        dataHandler.Close()
        if len(lst) > 0:
            ret = lst[0]
    except Exception as e:
        logger.error('Exception: %s', str(e))
    return ret

def GetAccountByUserName(userName):
    ret = None
    try:
        dataHandler = DataHandler()
        lst = dataHandler.GetAccountByUserName(userName)
        dataHandler.Close()
        if len(lst) > 0:
            ret = lst[0]
    except Exception as e:
        logger.error('Exception: %s', str(e))
    return ret


def GetUser(username):
    return IdentityManager.GetIdentityInfoFromDB(username)


def UpdateAce(userName, identityName, resourceType, resourceName, permissions):
    ret = None
    resourceAclPath = AuthorizationManager.GetResourceAclPath(resourceName, resourceType)
    if AuthorizationManager.HasAccess(userName, resourceType, resourceName, Permission.Admin):
        ret =  ACLManager.UpdateAce(identityName, resourceAclPath, permissions, 0)
    else:
        ret = "Access Denied!"
    return ret


def DeleteAce(userName, identityName, resourceType, resourceName):
    ret = None
    resourceAclPath = AuthorizationManager.GetResourceAclPath(resourceName, resourceType)
    if AuthorizationManager.HasAccess(userName, resourceType, resourceName, Permission.Admin):
        ret =  ACLManager.DeleteAce(identityName, resourceAclPath)
    else:
        ret = "Access Denied!"
    return ret


def AddStorage(userName, vcName, url, storageType, metadata, defaultMountPath):
    ret = None
    dataHandler = DataHandler()
    if AuthorizationManager.IsClusterAdmin(userName):
        ret =  dataHandler.AddStorage(vcName, url, storageType, metadata, defaultMountPath)
    else:
        ret = "Access Denied!"
    dataHandler.Close()
    return ret


def ListStorages(userName, vcName):
    ret = []
    dataHandler = DataHandler()
    if AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.User):
        ret = dataHandler.ListStorages(vcName)
    dataHandler.Close()
    return ret


def DeleteStorage(userName, vcName, url):
    ret = None
    dataHandler = DataHandler()
    if AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.Admin):
        ret =  dataHandler.DeleteStorage(vcName, url)
    else:
        ret = "Access Denied!"
    dataHandler.Close()
    return ret


def UpdateStorage(userName, vcName, url, storageType, metadata, defaultMountPath):
    ret = None
    dataHandler = DataHandler()
    if AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.Admin):
        ret =  dataHandler.UpdateStorage(vcName, url, storageType, metadata, defaultMountPath)
    else:
        ret = "Access Denied!"
    dataHandler.Close()
    return ret


def AddVC(userName, vcName, quota, metadata):
    ret = None
    dataHandler = DataHandler()
    if AuthorizationManager.IsClusterAdmin(userName):
        jobs = dataHandler.GetJobList("all", "all", num=None, status="killing")
        if len(jobs)>0:
            return "still have killing job"
        else:
            ret = dataHandler.DeleteJobByVc(vcName)
        ret =  dataHandler.AddVC(vcName, quota, metadata)
        if ret:
            cacheItem = {
                "vcName": vcName,
                "quota": quota,
                "metadata": metadata
            }
            with vc_cache_lock:
                # vc_cache[vcName] = cacheItem
                vc_cache.clear()
    else:
        ret = "Access Denied!"
    dataHandler.Close()
    return ret

def getClusterVCs(page=None,size=None,name=None):
    vcList = None
    try:
        if page and size:
            with vc_cache_lock:
                query_index = str(page)+str(size)
                if name:
                    query_index += name
                if query_index in vc_cache:
                    vcList = copy.deepcopy(vc_cache[query_index].values())
        else:
            if "all" in vc_cache:
                with vc_cache_lock:
                    vcList = copy.deepcopy(vc_cache["all"].values())
    except Exception:
        pass

    if not vcList:
        vcList = DataManager.ListVCs(page,size,name)
        tmp = {}
        for vc in vcList:
            tmp[vc["vcName"]] = vc

        if page and size:
            query_index = str(page) + str(size)
            if name:
                query_index += name
            with vc_cache_lock:
                vc_cache[query_index] = tmp
        else:
            with vc_cache_lock:
                vc_cache["all"] = tmp

    return vcList

def GetVcUserNum(vcName):
    ret = 0
    res = requests.get(url=config["usermanagerapi"] + "/vc/%s/user/count" % (vcName,),headers={"Authorization": "Bearer " + config["usermanagerapitoken"]})
    if res.status_code == 200:
        ret = res.json()["count"]
    return ret

def GetVcsUserCount():
    ret = {}
    res = requests.get(url=config["usermanagerapi"] + "/open/vc/user/name",headers={"Authorization": "Bearer " + config["usermanagerapitoken"]})
    if res.status_code == 200:
        ret = res.json()["vcUserNames"]
    return ret

def ListVCs(userName,page=None,size=None,name=None):
    ret = {"result":[]}
    vcList = getClusterVCs(page,size,name)
    vcCounts = GetVcsUserCount()

    for vc in vcList:
        if AuthorizationManager.HasAccess(userName, ResourceType.VC, vc["vcName"], Permission.User):
            vc['admin'] = AuthorizationManager.HasAccess(userName, ResourceType.VC, vc["vcName"], Permission.Admin)
            userList = vcCounts.get(vc["vcName"],[])
            vc["userNum"] = len(userList)
            vc["userNameList"] = userList
            ret["result"].append(vc)

    ret["totalNum"] = DataHandler().CountVCs(name)
    # web portal (client) can filter out Default VC
    return ret

def GetVCConfig(vcName):
    
    ret = {}
    ret["quota"]={}
    ret["user_quota"]={}

    config = DataHandler().GetVC(vcName)
    if config is None:
        return ret 
    else:
        pass
    
    if "quota" in config and len(config["quota"]) > 0:
        quota = json.loads(config["quota"])
        ret["quota"] = quota
    else:
        pass

    if "metadata" in config and len(config["metadata"]) > 0:
        user_quota = json.loads(config["metadata"])
        ret["user_quota"] = user_quota
    else:
        pass
    
    return ret

def ValidateDeviceRequest(devType, devNum, vcName):

    devType=devType.strip()
    devNum=int(devNum)

    userQuota=0
    msg = "success!"

    req_info= "devType: %s, devNum: %d, vcName: %s" % (devType, devNum, vcName)
    vc_config = GetVCConfig(vcName)

    if vc_config is None:
        msg = "vc not exists(%s)" %(vcName)
        logger.info(msg)
        return False, msg
    
    if "quota" not in vc_config:
        msg = "req(%s), incorrect vc config(%s)" %(req_info, str(vc_config))
        logger.info(msg)
        return False, msg

    if devType not in vc_config["quota"]:
        msg = "req(%s), target dev type not configed(%s)"% (req_info, str(vc_config))
        logger.info(msg)
        return False, msg

    configNum = int(vc_config["quota"][devType])
    if devNum > configNum:
        msg = "req(%s), request num(%d) more than vc configed(%d) " %(req_info, devNum, configNum)
        logger.info(msg)
        return False, msg

    if "user_quota" in vc_config:
        # not empty
        if bool(vc_config["user_quota"]):
            if devType not in vc_config["user_quota"]:
                msg = "req(%s), target dev type not included by user_quota(%s)" % (req_info, str(vc_config["user_quota"]))
                logger.info(msg)
                return False, msg
            
            elif "user_quota" in vc_config["user_quota"][devType] and int(vc_config["user_quota"][devType]["user_quota"]) < devNum:
                msg = "req(%s), request num(%d) more than user_quota(%d)" % (req_info, devNum, int(vc_config["user_quota"][devType]["user_quota"]))
                logger.info(msg)
                return False, msg
    else:
        pass

    return True, msg

def GetVC(userName, vcName):
    ret = None

    data_handler = DataHandler()

    cluster_status, _ = data_handler.GetClusterStatus()
    if not cluster_status or "gpu_capacity" not in cluster_status:
        return ret

    cluster_total = cluster_status["gpu_capacity"]
    cluster_available = cluster_status["gpu_avaliable"]
    cluster_reserved = cluster_status["gpu_reserved"]

    user_status = collections.defaultdict(lambda : ResourceInfo())
    user_status_preemptable = collections.defaultdict(lambda : ResourceInfo())

    vc_list = getClusterVCs()
    vc_info = {}
    vc_usage = collections.defaultdict(lambda :
            collections.defaultdict(lambda : 0))
    vc_preemptable_usage = collections.defaultdict(lambda :
            collections.defaultdict(lambda : 0))

    for vc in vc_list:
        vc_info[vc["vcName"]] = json.loads(vc["quota"])

    active_job_list = data_handler.GetActiveJobList()
    for job in active_job_list:
        jobParam = json.loads(base64.b64decode(job["jobParams"]))
        if "gpuType" in jobParam:
            if not jobParam["preemptionAllowed"]:
                vc_usage[job["vcName"]][jobParam["gpuType"]] += GetJobTotalGpu(jobParam)
            else:
                vc_preemptable_usage[job["vcName"]][jobParam["gpuType"]] += GetJobTotalGpu(jobParam)

    result = quota.calculate_vc_gpu_counts(cluster_total, cluster_available,
            cluster_reserved, vc_info, vc_usage)

    vc_total, vc_used, vc_available, vc_unschedulable = result

    for vc in vc_list:
        if vc["vcName"] == vcName and AuthorizationManager.HasAccess(userName, ResourceType.VC, vcName, Permission.User):

            num_active_jobs = {}
            for job in active_job_list:
                if job["vcName"] == vcName and job["jobStatus"] == "running":
                    username = job["userName"]
                    jobParam = json.loads(base64.b64decode(job["jobParams"]))
                    if not jobParam["gpuType"]:
                        continue
                    num_active_jobs.setdefault(jobParam["gpuType"],0)
                    num_active_jobs[jobParam["gpuType"]] += 1
                    if "gpuType" in jobParam:
                        if not jobParam["preemptionAllowed"]:
                            if username not in user_status:
                                user_status[username] = ResourceInfo()
                            user_status[username].Add(ResourceInfo({jobParam["gpuType"] : GetJobTotalGpu(jobParam)}))
                        else:
                            if username not in user_status_preemptable:
                                user_status_preemptable[username] = ResourceInfo()
                            user_status_preemptable[username].Add(ResourceInfo({jobParam["gpuType"] : GetJobTotalGpu(jobParam)}))

            vc["gpu_capacity"] = vc_total[vcName]
            vc["gpu_used"] = vc_used[vcName]
            vc["gpu_preemptable_used"] = vc_preemptable_usage[vcName]
            vc["gpu_unschedulable"] = vc_unschedulable[vcName]
            vc["gpu_avaliable"] = vc_available[vcName]
            vc["AvaliableJobNum"] = num_active_jobs
            vc["node_status"] = cluster_status["node_status"]
            vc["user_status"] = []
            for user_name, user_gpu in user_status.iteritems():
                # TODO: job_manager.getAlias should be put in a util file
                user_name = user_name.split("@")[0].strip()
                vc["user_status"].append({"userName":user_name, "userGPU":user_gpu.ToSerializable()})

            try:
                gpu_idle_url = config["gpu_reporter"] + '/gpu_idle'
                gpu_idle_params = {"vc": vcName}
                gpu_idle_response = requests.get(gpu_idle_url, params=gpu_idle_params)
                gpu_idle_json = gpu_idle_response.json()
                vc["gpu_idle"] = gpu_idle_json
            except Exception:
                logger.exception("Failed to fetch gpu_idle from gpu-exporter")
            vc["user_status_preemptable"] = []
            for user_name, user_gpu in user_status_preemptable.iteritems():
                user_name = user_name.split("@")[0].strip()
                vc["user_status_preemptable"].append({"userName": user_name, "userGPU": user_gpu.ToSerializable()})

            ret = vc
            break
    return ret


def GetJobTotalGpu(jobParams):
    numWorkers = 1
    if "numpsworker" in jobParams and jobParams["numpsworker"]:
        numWorkers = int(jobParams["numpsworker"])
    return int(jobParams["resourcegpu"]) * numWorkers

def DeleteVcRelate(vcName):
    ret = False
    res = requests.delete(url=config["usermanagerapi"] + "/open/vc/%s" %(vcName,),headers={"Authorization": "Bearer " + config["usermanagerapitoken"]})
    if res.status_code == 200:
        ret = True
    return ret


def DeleteVC(userName, vcName):
    dataHandler = DataHandler()
    if len(dataHandler.ListVCs())==1:
        return False
    if AuthorizationManager.IsClusterAdmin(userName):
        ret = DeleteVcRelate(vcName)
        if not ret:
            return "Delete relation error"
        jobs = dataHandler.GetJobList("all", vcName, num=None,status="running,scheduling,pausing")
        for job in jobs:
            dataHandler.UpdateJobTextFields({"jobId": job["jobId"],"vcName":vcName},{"jobStatus": "killing"})
        ret = dataHandler.DeleteJobByVcExcludeKilling(vcName)
        ret = dataHandler.DeleteTemplateByVc("vc:"+vcName)
        ret =  dataHandler.DeleteVC(vcName)
        if ret:
            with vc_cache_lock:
                # vc_cache.pop(vcName, None)
                vc_cache.clear()
    else:
        ret = "Access Denied!"
    dataHandler.Close()
    return ret


def UpdateVC(userName, vcName, quota, metadata):
    ret = None
    dataHandler = DataHandler()
    if AuthorizationManager.IsClusterAdmin(userName):
        ret =  dataHandler.UpdateVC(vcName, quota, metadata)
        if ret:
            cacheItem = {
                "vcName": vcName,
                "quota": quota,
                "metadata": metadata
            }
            with vc_cache_lock:
                # vc_cache[vcName] = cacheItem
                vc_cache.clear()
    else:
        ret = "Access Denied!"
    dataHandler.Close()
    return ret

def DettachVC(userName, vcName):

    ret = {}
    ret["code"] = 0

    # select all jobs from db
    dataHandler = DataHandler()

    global pendingStatus
    jobIds = dataHandler.GetUserJobs(userName, vcName, pendingStatus)
    dataHandler.Close()

    for jobItem in jobIds:
        if not KillJob(userName, jobItem["jobId"]):
            ret["code"] = -1
            ret["msg"] = "delete job(id: %s) failed" % (jobItem["jobId"])
        else:
            pass

    ret["msg"] = "success. %d job(s) deleted" % (len(jobIds))
    return ret

def GetAllDevice(userName):
    ret = {}
    if AuthorizationManager.IsClusterAdmin(userName):
        dataHandler = DataHandler()
        ret = dataHandler.GetAllDevice()
    return ret

def CountJobByStatus(userName,vcName,targetStatus):
    ret = -1
    if AuthorizationManager.IsClusterAdmin(userName):
        dataHandler = DataHandler()
        ret = dataHandler.CountJobByStatus(vcName,targetStatus)
    return ret

def GetEndpoints(userName, jobId):
    dataHandler = DataHandler()
    ret = []
    try:
        job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName", "endpoints","jobDescription"])
        if job is not None:
            if job["userName"] == userName or AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Admin):
                endpoints = {}
                if job["endpoints"] is not None:
                    endpoints = json.loads(job["endpoints"])
                for [_, endpoint] in endpoints.items():
                    epItem = EndpointUtils.parse_endpoint(endpoint,job)
                    ret.append(epItem)
    except Exception as e:
        logger.error("Get endpoint exception, ex: %s", str(e))
    finally:
        dataHandler.Close()
    return ret

def UpdateEndpoints(userName, jobId, requested_endpoints, arguments, interactive_ports):
    dataHandler = DataHandler()
    try:
        job = dataHandler.GetJobTextFields(jobId, ["userName", "vcName", "jobParams", "endpoints"])
        if job is None:
            msg = "Job %s cannot be found in database" % jobId
            logger.error(msg)
            return msg, 404
        if job["userName"] != userName and (not AuthorizationManager.HasAccess(userName, ResourceType.VC, job["vcName"], Permission.Admin)):
            msg = "You are not authorized to enable endpoint for job %s" % jobId
            logger.error(msg)
            return msg, 403

        job_params = json.loads(base64.b64decode(job["jobParams"]))
        job_type = job_params["jobtrainingtype"]
        job_endpoints = {}
        if job["endpoints"] is not None:
            job_endpoints = json.loads(job["endpoints"])

        # get pods
        pod_names = []
        if job_type == "RegularJob":
            pod_names.append(jobId)
        else:
            if "numps" in job_params and "numpsworker" in job_params:
                nums = {"ps": int(job_params["numps"]), "worker": int(job_params["numpsworker"])}
            else:
                nums = {"ps": 0, "worker": 1}
            for role in ["ps", "worker"]:
                for i in range(nums[role]):
                    pod_names.append(jobId + "-" + role + str(i))

        # HostNetwork
        if "hostNetwork" in job_params and job_params["hostNetwork"] == True:
            host_network = True
        else:
            host_network = False

        # username
        username = getAlias(job["userName"])

        endpoints = job_endpoints

        if "ssh" in requested_endpoints:
            # setup ssh for each pod
            for pod_name in pod_names:
                endpoint_id = "e-" + pod_name + "-ssh"

                if endpoint_id in job_endpoints:
                    logger.info("Endpoint %s exists. Skip.", endpoint_id)
                    continue
                logger.info("Endpoint %s does not exist. Add.", endpoint_id)

                endpoint = {
                    "id": endpoint_id,
                    "jobId": jobId,
                    "podName": pod_name,
                    "username": username,
                    "name": "ssh",
                    "status": "pending",
                    "hostNetwork": host_network,
                    "arguments": arguments
                }
                endpoints[endpoint_id] = endpoint

        # Only open Jupyter on the master
        if 'ipython' in requested_endpoints:
            if job_type == "RegularJob":
                pod_name = pod_names[0]
            else:
                # For a distributed job, we set up jupyter on first worker node.
                # PS node does not have GPU access.
                # TODO: Simplify code logic after removing PS
                pod_name = pod_names[1]

            endpoint_id = "e-" + jobId + "-ipython"

            if endpoint_id not in job_endpoints:
                logger.info("Endpoint %s does not exist. Add.", endpoint_id)
                endpoint = {
                    "id": endpoint_id,
                    "jobId": jobId,
                    "podName": pod_name,
                    "username": username,
                    "name": "ipython",
                    "status": "pending",
                    "hostNetwork": host_network,
                    "arguments": arguments
                }
                endpoints[endpoint_id] = endpoint
            else:
                logger.info("Endpoint %s exists. Skip.", endpoint_id)

        # Only open tensorboard on the master
        if 'tensorboard' in requested_endpoints:
            if job_type == "RegularJob":
                pod_name = pod_names[0]
            else:
                # For a distributed job, we set up jupyter on first worker node.
                # PS node does not have GPU access.
                # TODO: Simplify code logic after removing PS
                pod_name = pod_names[1]

            endpoint_id = "e-" + jobId + "-tensorboard"

            if endpoint_id not in job_endpoints:
                logger.info("Endpoint %s does not exist. Add.", endpoint_id)
                endpoint = {
                    "id": endpoint_id,
                    "jobId": jobId,
                    "podName": pod_name,
                    "username": username,
                    "name": "tensorboard",
                    "status": "pending",
                    "hostNetwork": host_network,
                    "arguments": arguments
                }
                endpoints[endpoint_id] = endpoint
            else:
                logger.info("Endpoint %s exists. Skip.", endpoint_id)

        # Only open vscode on the master
        if 'vscode' in requested_endpoints:
            if job_type == "RegularJob":
                pod_name = pod_names[0]
            else:
                # For a distributed job, we set up jupyter on first worker node.
                # PS node does not have GPU access.
                # TODO: Simplify code logic after removing PS
                pod_name = pod_names[1]

            endpoint_id = "e-" + jobId + "-vscode"

            if endpoint_id not in job_endpoints:
                logger.info("Endpoint %s does not exist. Add.", endpoint_id)
                endpoint = {
                    "id": endpoint_id,
                    "jobId": jobId,
                    "podName": pod_name,
                    "username": username,
                    "name": "vscode",
                    "status": "pending",
                    "hostNetwork": host_network
                }
                endpoints[endpoint_id] = endpoint
            else:
                logger.info("Endpoint %s exists. Skip.", endpoint_id)

        # interactive port
        for interactive_port in interactive_ports:
            if job_type == "RegularJob":
                pod_name = pod_names[0]
            else:
                # For a distributed job, we set up jupyter on first worker node.
                # PS node does not have GPU access.
                # TODO: Simplify code logic after removing PS
                pod_name = pod_names[1]

            endpoint_id = "e-" + jobId + "-port-" + str(interactive_port["podPort"])
            if endpoint_id not in job_endpoints:
                logger.info("Endpoint %s does not exist. Add.", endpoint_id)
                endpoint = {
                    "id": endpoint_id,
                    "jobId": jobId,
                    "podName": pod_name,
                    "username": username,
                    "name": interactive_port["name"],
                    "podPort": interactive_port["podPort"],
                    "status": "pending",
                    "hostNetwork": host_network,
                    "arguments": arguments
                }
                endpoints[endpoint_id] = endpoint
            else:
                logger.info("Endpoint %s exists. Skip.", endpoint_id)

        dataHandler.UpdateJobTextField(jobId, "endpoints", json.dumps(endpoints))
        return endpoints, 200
    except Exception as e:
        logger.error("Get endpoint exception, ex: %s", str(e))
    finally:
        dataHandler.Close()
    return "server error", 500



def get_job(job_id):
    data_handler = None
    try:
        data_handler = DataHandler()
        jobs = data_handler.GetJob(jobId=job_id)
        if len(jobs) == 1:
            return jobs[0]
    except Exception as e:
        logger.error("Exception in get_job: %s" % str(e))
    finally:
        if data_handler is not None:
            data_handler.Close()
    return None


def update_job(job_id, field, value):
    dataHandler = DataHandler()
    dataHandler.UpdateJobTextField(job_id, field, value)
    dataHandler.Close()


def get_job_priorities():
    dataHandler = DataHandler()
    job_priorites = dataHandler.get_job_priority()
    dataHandler.Close()
    return job_priorites


def update_job_priorites(username, job_priorities):
    data_handler = None
    try:
        data_handler = DataHandler()

        # Only job owner and VC admin can update job priority.
        # Fail job priority update if there is one unauthorized items.
        pendingJobs = {}
        for job_id in job_priorities:
            priority = job_priorities[job_id]
            job = data_handler.GetJobTextFields(job_id, ["userName", "vcName", "jobStatus"])
            if job is None:
                continue

            vc_admin = AuthorizationManager.HasAccess(username, ResourceType.VC, job["vcName"], Permission.Admin)
            if job["userName"] != username and (not vc_admin):
                return False

            # Adjust priority based on permission
            permission = Permission.Admin if vc_admin else Permission.User
            job_priorities[job_id] = adjust_job_priority(priority, permission)

            if job["jobStatus"] in pendingStatus.split(","):
                pendingJobs[job_id] = job_priorities[job_id]

        ret_code = data_handler.update_job_priority(job_priorities)
        return ret_code, pendingJobs

    except Exception as e:
        logger.error("Exception when updating job priorities: %s" % e)

    finally:
        if data_handler is not None:
            data_handler.Close()

def getAlias(username):
    if "@" in username:
        username = username.split("@")[0].strip()
    if "/" in username:
        username = username.split("/")[1].strip()
    return username

def ConvertDataFormat(projectId, datasetId,datasetType,targetFormat):
    data_handler = None
    if datasetType is None:
        datasetType = "image"
    if targetFormat is None:
        targetFormat = "coco"
    try:
        data_handler = DataHandler()
        ret = data_handler.ConvertDataFormat(projectId, datasetId,datasetType,targetFormat)
        return ret
    except Exception as e:
        logger.error("Exception in ConvertDataFormat: %s" % str(e))
    finally:
        if data_handler is not None:
            data_handler.Close()
    return None

def GetConvertList(status):
    data_handler = None
    try:
        data_handler = DataHandler()
        ret = data_handler.getConvertList(status)
        return ret
    except Exception as e:
        logger.error("Exception in ConvertDataFormat: %s" % str(e))
    finally:
        if data_handler is not None:
            data_handler.Close()
    return None

def GetConvertDetail(projectId,datasetId):
    data_handler = None
    try:
        data_handler = DataHandler()
        ret = data_handler.GetConvertDetail(projectId,datasetId)
        if len(ret)>0:
            return ret[0]
    except Exception as e:
        logger.error("Exception in ConvertDataFormat: %s" % str(e))
    finally:
        if data_handler is not None:
            data_handler.Close()
    return None


def GetJobSummary(userName, jobType, vcName):
    data_handler = None

    try:
        data_handler = DataHandler()
        summary = data_handler.get_job_summary(userName, jobType, vcName)
        return summary

    except Exception as e:
        logger.error("Exception in ConvertDataFormat: %s" % str(e))

    finally:
        if data_handler is not None:
            data_handler.Close()
        else:
            pass

    return None






def GetVersionInfo():
    
    if ( os.path.isfile('/version-info')):
        with open('/version-info') as f:
            all_version = yaml.load(f.read())
            current = {}
            current["name"] = all_version[0]['version']
            current['description'] = all_version[0]['description']
            current['updateAt'] = all_version[0]['updateAt']
            history = []
            for versionInfo in all_version[1:]:
                version_info = {}
                version_info["name"] = versionInfo['version']
                version_info['description'] = versionInfo['description']
                version_info['updateAt'] = versionInfo['updateAt']
                history.append(version_info)
        return current, history
    else:
        logger.error("Exception in reading version file: file not exist")
    return None, None

if __name__ == '__main__':
    TEST_SUB_REG_JOB = False
    TEST_JOB_STATUS = True
    TEST_DEL_JOB = False
    TEST_GET_TB = False
    TEST_GET_SVC = False
    TEST_GET_LOG = False

    if TEST_SUB_REG_JOB:
        parser = argparse.ArgumentParser(description='Launch a kubernetes job')
        parser.add_argument('-f', '--param-file', required=True, type=str,
                            help = 'Path of the Parameter File')
        parser.add_argument('-t', '--template-file', required=True, type=str,
                            help = 'Path of the Job Template File')
        args, unknown = parser.parse_known_args()
        with open(args.param_file,"r") as f:
            jobParamsJsonStr = f.read()
        f.close()

        SubmitRegularJob(jobParamsJsonStr,args.template_file)

    if TEST_JOB_STATUS:
        print GetJobStatus(sys.argv[1])

    if TEST_DEL_JOB:
        print DeleteJob("tf-dist-1483504085-13")

    if TEST_GET_TB:
        print GetTensorboard("tf-resnet18-1483509537-31")

    if TEST_GET_SVC:
        print GetServiceAddress("tf-i-1483566214-12")

    if TEST_GET_LOG:
        print GetLog("tf-i-1483566214-12")
