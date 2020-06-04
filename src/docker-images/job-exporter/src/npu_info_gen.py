#!/usr/bin/env python3

import argparse
import logging
import os
import json
import sys
import threading
import signal
import faulthandler
import gc
import datetime

import time
import pdb

logger = logging.getLogger(__name__)

import subprocess

import logging


def exec_cmd(*args, **kwargs):
    """ exec a cmd with timeout, also record time used using prometheus higtogram """
    if kwargs.get("histogram") is not None:
        histogram = kwargs.pop("histogram")
    else:
        histogram = None

    logger.debug("about to exec %s", args[0])

    if histogram is not None:
        with histogram.time():
            return subprocess.check_output(*args, **kwargs).decode("utf-8")
    else:
        return subprocess.check_output(*args, **kwargs).decode("utf-8")


def exec_shell_cmd(cmd):

    logger.debug("about to exec %s", cmd)
    output = ""
    print(cmd)

    try:
        output = subprocess.check_output(
            cmd, stderr=subprocess.STDOUT, shell=True, 
            universal_newlines=True)
    except subprocess.CalledProcessError as exc:
        print("Status : FAIL", exc.returncode, exc.output)

    return output

def parse_npu_number_smi_output(npu_number_smi_output):
    lines = npu_number_smi_output.split("\n\t")[1:]
    numbers = []
    for one_line in lines:
        NPU_ID,Chip_ID,Chip_Logic_ID,Chip_Name =  filter(lambda x:x!="" and x!="\n",one_line.split(" "))
        numbers.append(NPU_ID)
    return numbers


def gen_smiinfo():
    ## create log folder
    npu_smi_dir = "/var/log/npu/npu_smi"
    exec_shell_cmd("mkdir -p " + npu_smi_dir)

    # create file for specific command 
    # cmd1
    device_list_cmd = "npu-smi info -m"
    device_list_file = npu_smi_dir + "/device_list"
    device_list_info = exec_shell_cmd(device_list_cmd)

    with open(device_list_file, "w+") as f:
        f.write(device_list_info)
        f.close()

    # cmd2
    # parse device list
    npu_numbers = parse_npu_number_smi_output(device_list_info)
    for one_npu_id in npu_numbers:
        device_info_cmd = "npu-smi info -t common -i " + str(one_npu_id)
        device_info_file = npu_smi_dir + "/device" + str(one_npu_id)
        
        device_output = exec_shell_cmd(device_info_cmd)
        with open(device_info_file, "w+") as f:
            f.write(device_output)
            f.close()

    return 

if __name__ == "__main__":
    while True:
        #pdb.set_trace()
        gen_smiinfo()
        time.sleep(1)
