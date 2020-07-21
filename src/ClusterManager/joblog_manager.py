import json
import os
import time
import argparse
import uuid
import subprocess
import sys
import datetime

import yaml
from jinja2 import Environment, FileSystemLoader, Template
import base64

import re

import thread
import threading
import random

import textwrap
import logging
import logging.config

from multiprocessing import Process, Manager

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../storage"))
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../utils"))

from jobs_tensorboard import GenTensorboardMeta
import k8sUtils
from osUtils import mkdirsAsUser
from config import config, GetStoragePath
from DataHandler import DataHandler

from cluster_manager import setup_exporter_thread, manager_iteration_histogram, register_stack_trace_dump, update_file_modification_time, record

logger = logging.getLogger(__name__)

def create_log(logdir = '/var/log/dlworkspace'):
    if not os.path.exists( logdir ):
        os.system("mkdir -p " + logdir)
    with open('logging.yaml') as f:
        logging_config = yaml.load(f)
        f.close()
        logging_config["handlers"]["file"]["filename"] = logdir+"/joblogmanager.log"
        logging.config.dictConfig(logging_config)

def save_log(jobLogDir,containerID,userId,log,order=1):
    try:
        containerLogPath = os.path.join(jobLogDir, "log-container-" + containerID + ".txt"+"."+str(order))
        with open(containerLogPath, 'w') as f:
            f.write(log)
        f.close()
        os.system("chown -R %s %s" % (userId, containerLogPath))
    except Exception as e:
        logger.exception("write container log failed")

@record
def extract_job_log(jobId,logPath,userId):
    try:
        dataHandler = DataHandler()

        # logs = k8sUtils.GetLog(jobId)
        # logs = k8sUtils.getJobConsoleDetail(jobId)
        jupyterLog = k8sUtils.getJupyterInfo(jobId)
    
        # TODO: Replace joblog manager with elastic search
        logs = k8sUtils.GetLog(jobId, tail=None)

        # Do not overwrite existing logs with empty log
        # DLTS bootstrap will generate logs for all containers.
        # If one container has empty log, skip writing.
        for log in logs:
            if "containerLog" in log and log["containerLog"] == "":
                return

        jobLogDir = os.path.dirname(logPath)
        if not os.path.exists(jobLogDir):
            mkdirsAsUser(jobLogDir,userId)
        logStr = ""
        trimlogstr = ""


        for log in logs:
            if "podName" in log and "containerID" in log and "containerLog" in log:
                logStr += "=========================================================\n"
                logStr += "=========================================================\n"
                logStr += "=========================================================\n"
                logStr += "        logs from pod: %s\n" % log["podName"]
                logStr += "=========================================================\n"
                logStr += "=========================================================\n"
                logStr += "=========================================================\n"
                logStr += log["containerLog"]
                logStr += jupyterLog
                logStr += "\n\n\n"
                logStr += "=========================================================\n"
                logStr += "        end of logs from pod: %s\n" % log["podName"]
                logStr += "=========================================================\n"
                logStr += "\n\n\n"

        logLines = logStr.split('\n')
        length = len(logLines)
        if (length <= 2000):
            save_log(jobLogDir,str(jobId),userId,logStr)
        else:
            with open(os.path.join(jobLogDir,"max_page"), 'w') as f:
                f.write(str(length//2000+1))
            for i in range(1,length//2000+2):
                trimlogstr = "\n".join(logLines[(i-1)*2000:i*2000])
                save_log(jobLogDir, str(jobId), userId, trimlogstr,i)
    except Exception as e:
        logger.error(e)


def update_job_logs():
    while True:
        try:
            dataHandler = DataHandler()
            pendingJobs = dataHandler.GetPendingJobs()
            for job in pendingJobs:
                try:
                    if job["jobStatus"] == "running" :
                        logger.info("updating job logs for job %s" % job["jobId"])
                        jobParams = json.loads(base64.b64decode(job["jobParams"]))
                        jobPath,workPath,dataPath = GetStoragePath(jobParams["jobPath"],jobParams["workPath"],jobParams["dataPath"])
                        localJobPath = os.path.join(config["storage-mount-path"],jobPath)
                        logPath = os.path.join(localJobPath,"logs/joblog.txt")

                        extract_job_log(job["jobId"],logPath,jobParams["userId"])
                except Exception as e:
                    logger.exception("handling logs from %s", job["jobId"])
        except Exception as e:
            logger.exception("get pending jobs failed")

        time.sleep(1)



def Run():
    register_stack_trace_dump()
    create_log()
    logger.info("start to update job logs ...")

    while True:
        update_file_modification_time("joblog_manager")

        with manager_iteration_histogram.labels("joblog_manager").time():
            try:
                update_job_logs()
            except Exception as e:
                logger.exception("update job logs failed")
        time.sleep(1)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", "-p", help="port of exporter", type=int, default=9203)
    args = parser.parse_args()
    setup_exporter_thread(args.port)

    Run()
