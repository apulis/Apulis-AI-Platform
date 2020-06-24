#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import json
import time
import pdb


if __name__ == "__main__":

    done = 0
    rank_id = 0
    hccl_data = {}

    # for test only
    #os.environ['DLWS_WORKER_NUM'] = "2"
    #os.environ['DLWS_JOB_ID'] = "test_npu_device"
    #os.environ['DLWS_USER_NAME'] = "bifeng.peng"
    #

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
    group["instance_count"] = worker_num * 8
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

    print("succ!")
    hccl_data["group_list"].append(group)
    # dump to json file
    with open(npu_dir + '/hccl.json', 'w') as fp:
        json.dump(hccl_data, fp)

    pass
