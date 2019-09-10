import json
import os
import time
import argparse
import uuid
import subprocess
import sys
import datetime
import copy


sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../storage"))
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../utils"))

import k8sUtils

import yaml
from DataHandler import DataHandler

import logging
import logging.config


def create_log( logdir = '/var/log/dlworkspace' ):
    if not os.path.exists( logdir ):
        os.system("mkdir -p " + logdir )
    with open('logging.yaml') as f:
        logging_config = yaml.load(f)
        f.close()
        logging_config["handlers"]["file"]["filename"] = logdir+"/commandmanager.log"
        logging.config.dictConfig(logging_config)

def RunCommand(command):
    dataHandler = DataHandler()
    logging.info("Job %s exec command: [%s]" % (command["jobId"], command["command"]))
    output = k8sUtils.kubectl_exec("exec %s %s" % (command["jobId"], command["command"]))
    logging.info("exec output:\n %s" % (output))
    dataHandler.FinishCommand(command["id"], output)
    dataHandler.Close()
    return True

def Run():
    create_log()
    logging.info("start to update command information ...")
    while True:
        try:
            dataHandler = DataHandler()
            pendingCommands = dataHandler.GetPendingCommands()
            for command in pendingCommands:
                try:
                    RunCommand(command)
                except Exception as e:
                    logging.error(e)
        except Exception as e:
            logging.error(e)
        time.sleep(1)

if __name__ == '__main__':
    Run()