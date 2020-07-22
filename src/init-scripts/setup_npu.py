#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import json
import time
import pdb


def generate_mindspore():
    
    done = 0
    rank_id = 0
    hccl_data = {}

    # for test only
    #os.environ['DLWS_WORKER_NUM'] = "2"
    #os.environ['DLWS_JOB_ID'] = "test_npu_device"
    #os.environ['DLWS_USER_NAME'] = "bifeng.peng"
    #

    ## non distributed job
    if "DLWS_WORKER_NUM" not in os.environ:
        os.environ['DLWS_WORKER_NUM'] = "1"
    else:
        pass

    worker_num = int(os.environ['DLWS_WORKER_NUM'])
    job_id = os.environ['DLWS_JOB_ID']
    user_name =  os.environ['DLWS_USER_NAME']
    npu_dir = '/home/%s/.npu/%s/' % (user_name, job_id)

    hccl_data["board_id"] = "0x0020"
    hccl_data["chip_info"] = "910"
    hccl_data["deploy_mode"] = "lab"
    hccl_data["group_count"] = "1"
    hccl_data["para_plane_nic_location"] = "device"
    hccl_data["para_plane_nic_name"] = [
                                        "eth0",
                                        "eth1",
                                        "eth2",
                                        "eth3",
                                        "eth4",
                                        "eth5",
                                        "eth6",
                                        "eth7"
                                    ]
    hccl_data["para_plane_nic_num"] = 8
    hccl_data["status"] = "completed"
    hccl_data["group_list"] = []

    group = {}
    group["device_num"] = worker_num * 8
    group["server_num"] = worker_num
    group["group_name"] = "test"
    group["instance_count"] = group["device_num"] 
    group["instance_list"] = []

    while True:

        PATH = npu_dir + ('/npu_%d.info' % (done))
        if os.path.isfile(PATH) and os.access(PATH, os.R_OK):

            with open(PATH, "r") as f:

                ips = ""
                host_ip = ""

                # get ips and host info from file
                for line in f:
                    print(line)
                    if "ip=" in line:
                        _, ips = line.strip().split("=")
                    elif "host=" in line:
                        _, host_ip = line.strip().split("=")

                # parse string to get all device ips
                ip_list = ips.split(",")
                for ip_elem in ip_list:

                    # one device
                    device_id, device_ip = ip_elem.split(":")

                    ## set up group list
                    device_item = {}  # item of instance list
                    device_item["devices"] = {
                        "device_id" : device_id,
                        "device_ip" : device_ip
                    }

                    device_item["rank_id"] = rank_id
                    device_item["server_id"] = host_ip

                    # append to instance list
                    rank_id = rank_id + 1
                    #pdb.set_trace()
                    group["instance_list"].append(device_item)

                f.close()
                done = done + 1
        else:
            pass

        if done == worker_num:
            break
        else:
            pass

        time.sleep(1)

    group["instance_count"] = group["device_num"] = len(group["instance_list"])
    print("succ!")
    hccl_data["group_list"].append(group)

    # dump to json file
    with open(npu_dir + '/hccl.json', 'w') as fp:
        json.dump(hccl_data, fp)

    return

def generate_tensorflow():
    
    done = 0          # worker node to process
    rank_id = 0       # equals to device count
    hccl_data = {}

    # for test only
    #os.environ['DLWS_WORKER_NUM'] = "2"
    #os.environ['DLWS_JOB_ID'] = "test_npu_device"
    #os.environ['DLWS_USER_NAME'] = "bifeng.peng"
    #

    ## non distributed job
    if "DLWS_WORKER_NUM" not in os.environ:
        os.environ['DLWS_WORKER_NUM'] = "1"
    else:
        pass

    worker_num = int(os.environ['DLWS_WORKER_NUM'])
    job_id = os.environ['DLWS_JOB_ID']
    pod_name = os.environ['POD_NAME']
    user_name =  os.environ['DLWS_USER_NAME']
    npu_dir = '/home/%s/.npu/%s/' % (user_name, job_id)

    hccl_data["group_count"] = "1"
    hccl_data["status"] = "completed"
    hccl_data["group_list"] = []

    group = {}
    #group["device_count"] = worker_num * 8
    group["instance_count"] = worker_num
    group["group_name"] = "test"
    group["instance_count"] = group["device_num"] 
    group["instance_list"] = []

    while True:

        PATH = npu_dir + ('/npu_%d.info' % (done))
        if os.path.isfile(PATH) and os.access(PATH, os.R_OK):

            with open(PATH, "r") as f:

                ips = ""
                host_ip = ""

                # get ips and host info from file
                for line in f:
                    print(line)
                    if "ip=" in line:
                        _, ips = line.strip().split("=")
                    elif "host=" in line:
                        _, host_ip = line.strip().split("=")

                instance_item = {}  # item of instance list
                instance_item["pod_name"] = pod_name
                instance_item["server_id"] = host_ip
                instance_item["devices"] = []

                # parse string to get all device ips
                ip_list = ips.split(",")
                for ip_elem in ip_list:

                    # one device
                    device_id, device_ip = ip_elem.split(":")

                    ## set up group list
                    device_item =  {
                        "device_id" : device_id,
                        "device_ip" : device_ip
                    }

                    # append to instance list
                    rank_id = rank_id + 1
                    instance_item["devices"].append(device_item)
                    #pdb.set_trace()
                
                group["instance_list"].append(instance_item)
                f.close()
                done = done + 1
                
        else:
            pass

        if done == worker_num:
            break
        else:
            pass

        time.sleep(1)

    group["device_count"] = rank_id
    group["instance_count"] = len(group["instance_list"])
    hccl_data["group_list"].append(group)

    print("succ!")

    # dump to json file
    with open(npu_dir + '/hccl.json', 'w') as fp:
        json.dump(hccl_data, fp)

    return


if __name__ == "__main__":

    generate_mindspore()
    generate_tensorflow()
    pass
