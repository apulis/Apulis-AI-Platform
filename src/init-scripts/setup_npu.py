#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import json
import time
import pdb
import platform
import string
import random


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
    hccl_data["para_plane_nic_num"] = "8"
    hccl_data["status"] = "completed"
    hccl_data["group_list"] = []

    group = {}
    group["device_num"] = str(worker_num * 8)
    group["server_num"] = str(worker_num)
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
                ip_list = sorted(ip_list)

                for ip_elem in ip_list:

                    # one device
                    device_id, device_ip = ip_elem.split(":")

                    ## set up group list
                    device_item = {}  # item of instance list
                    device_item["devices"] = [{
                        "device_id" : device_id,
                        "device_ip" : device_ip
                    }]

                    device_item["rank_id"] = str(rank_id)
                    device_item["server_id"] = str(host_ip)

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

    group["instance_count"] = group["device_num"] = str(len(group["instance_list"]))
    print("succ!")
    hccl_data["group_list"].append(group)

    # dump to json file
    with open(npu_dir + '/hccl_ms.json', 'w') as fp:
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
    group["instance_count"] = str(worker_num)
    group["group_name"] = "test"
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
                if worker_num > 1:
                    instance_item["pod_name"] = pod_name + "_" + str(done)
                else:
                    instance_item["pod_name"] = pod_name

                instance_item["server_id"] = host_ip
                instance_item["devices"] = []

                # parse string to get all device ips
                ip_list = ips.split(",")
                ip_list = sorted(ip_list)

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

    group["device_count"] = str(rank_id)
    group["instance_count"] = str(len(group["instance_list"]))
    hccl_data["group_list"].append(group)

    print("succ!")

    # dump to json file
    with open(npu_dir + '/hccl_tf.json', 'w') as fp:
        json.dump(hccl_data, fp)

    return


# 从/pod.env导入字典数据集
def load_env(file_path):
    envs = {}

    with open(file_path, "r") as f:

        lines = f.readlines()
        for line in lines:

            line = line.strip().lstrip("export")
            if line is not "" and "=" in line:
                key_val = line.strip().split("=")

                key = key_val[0]
                value = key_val[1]
                envs[key] = value

            else:
                pass

        f.close()

    return envs

# 向/pod.env写入字典数据，先判断是否存在此环境量
# 如果已存在，则覆盖此变量
def add_env(path, envs):

    # 覆盖相同key数据，文件已有的key保持不变
    envs_orig = load_env(path)
    for k, v in envs.items():
         envs_orig[k] = v

    with open(path, "w") as f:
        for k, v in envs_orig.items():
            f.write("export %s=%s\n" % (k, v))
        
        f.close()

    return


def get_os_flag():

    osflag="x86-64"

    if platform.machine() == "aarch64":
        osflag = "arm64"
    else:
        pass

    return osflag

def get_gnu_arch_flag():

    osflag="x86-64"

    if platform.machine() == "aarch64":
        osflag = "aarch64"
    else:
        pass

    return osflag


def get_random_num(length):
    return ''.join(random.choice(string.digits) for _ in range(length))

def set_bashrc(username):

    path = ""
    if username == "root":
        path = "/root/.bashrc"
    else:
        path = "/home/" + username + "/.bashrc"


    with open(path, "a") as f:

        cmd = '''
        if [ -f "/pod.env"  ]; then
            . /pod.env
        fi
        '''

        f.write(cmd + "\n")
        f.close()

    return


def handle_mindspore():

    path = "/pod.env"
    envs = load_env(path)
    envs_to_add= {}
    envs_to_add["DEVICE_ID"] = "0"

    if "VISIBLE_IDS" in envs:
        envs["VISIBLE_IDS"] = envs["VISIBLE_IDS"].replace("\\","")
        envs_to_add["VISIBLE_IDS"] = envs["VISIBLE_IDS"] 
    else:
        pass


    if "NPU_IPS" in envs:
        envs["NPU_IPS"] = envs["NPU_IPS"].replace("\\","")
        envs_to_add["NPU_IPS"] = envs["NPU_IPS"] 
    else:
        pass

    ## 将pod.env已有的环境变量
    ## 与os当前具有的环境变量合并, 放入envs
    for k, v in os.environ.items():
        if k not in envs:
            envs[k] = v
        else:
            pass

    ## 不需要解析device id

    ## 设置随机参数
    envs["RANDOM"] = get_random_num(6)
    envs["osflag"] = get_os_flag()
    envs["gnu_arch"] = get_gnu_arch_flag()

    tensorflow_envs = [
        "PYTHONPATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:${PYTHONPATH}",
        "LD_LIBRARY_PATH=/usr/lib/${gnu_arch}-linux-gnu/hdf5/serial:/usr/local/Ascend/add-ons:/home/HwHiAiUser/Ascend/nnae/latest/fwkacllib/lib64:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/atc/lib64:$LD_LIBRARY_PATH",
        "TBE_IMPL_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe",
        "PATH=$PATH:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/fwkacllib/ccec_compiler/bin/",
        "ASCEND_OPP_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp",

        "SOC_VERSION=Ascend910",
        "RANK_SIZE=1",
        "POD_NAME=${DLWS_JOB_ID}",
        "JOB_ID=${RANDOM}"
    ]


    for item in tensorflow_envs:

        tpl = string.Template(item)
        new_item = tpl.safe_substitute(envs)

        if "=" in new_item:
            key_val = new_item.strip().split("=")
            k = key_val[0]
            v = key_val[1]
            envs_to_add[k] = v

        else:
            pass

    add_env(path, envs_to_add)

    ## 生成shell脚本
    pod_cmd = os.environ["DLWS_LAUNCH_CMD"]
    npu_info_dir = "/home/" + os.environ["DLWS_USER_NAME"] + "/.npu/" + os.environ["DLWS_JOB_ID"] + "/train.sh"

    cmd = 'python /pod/scripts/setup_huawei.py --type mindspore --command "%s" --out %s'% (pod_cmd, npu_info_dir)
    os.system(cmd)
    os.system("chmod 777 " + npu_info_dir)

    set_bashrc("root")
    generate_mindspore()

    return


def handle_tensorflow():

    path = "/pod.env"
    
    envs = load_env(path)
    envs_to_add= {}

    if "VISIBLE_IDS" in envs:
        envs["VISIBLE_IDS"] = envs["VISIBLE_IDS"].replace("\\","")
        envs_to_add["VISIBLE_IDS"] = envs["VISIBLE_IDS"] 
    else:
        pass

    if "NPU_IPS" in envs:
        envs["NPU_IPS"] = envs["NPU_IPS"].replace("\\","")
        envs_to_add["NPU_IPS"] = envs["NPU_IPS"] 
    else:
        pass

    ## 将pod.env已有的环境变量
    ## 与os当前具有的环境变量合并
    for k, v in os.environ.items():
        if k not in envs:
            envs[k] = v
        else:
            pass

    ## 第一个设备id
    device_id="0"
    device_index="0"

    if "VISIBLE_IDS" in envs:
         devid = envs["VISIBLE_IDS"].split(",")[0].strip()
         if len(devid) > 0:
             device_id = devid
         else:
             pass
    else:
        pass

    device_index = device_id

    ## 设置随机参数
    envs["RANDOM"] = get_random_num(6)
    envs["osflag"] = get_os_flag()
    envs["gnu_arch"] = get_gnu_arch_flag()


    tensorflow_envs = [
        "PYTHONPATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:${PYTHONPATH}",
        "LD_LIBRARY_PATH=/usr/lib/${gnu_arch}-linux-gnu/hdf5/serial:/usr/local/Ascend/add-ons:/home/HwHiAiUser/Ascend/nnae/latest/fwkacllib/lib64:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/atc/lib64:$LD_LIBRARY_PATH",
        "TBE_IMPL_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe",
        "PATH=$PATH:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/fwkacllib/ccec_compiler/bin/",
        "ASCEND_OPP_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp",

        "SOC_VERSION=Ascend910",
        "RANK_SIZE=1",
        "POD_NAME=${DLWS_JOB_ID}",
        "JOB_ID=${RANDOM}"
    ]

    envs_to_add["DEVICE_ID"] = device_id
    envs_to_add["DEVICE_INDEX"] = device_index

    for item in tensorflow_envs:

        tpl = string.Template(item)
        new_item = tpl.safe_substitute(envs)

        if "=" in new_item:
            key_val = new_item.strip().split("=")
            k = key_val[0]
            v = key_val[1]
            envs_to_add[k] = v

        else:
            pass

    add_env(path, envs_to_add)

    ## 生成shell脚本
    pod_cmd = os.environ["DLWS_LAUNCH_CMD"]
    npu_info_dir = "/home/" + os.environ["DLWS_USER_NAME"] + "/.npu/" + os.environ["DLWS_JOB_ID"] + "/train.sh"

    cmd = 'python /pod/scripts/setup_huawei.py --type tensorflow --command "%s" --out %s'% (pod_cmd, npu_info_dir)
    print(cmd, "==========================")
    os.system(cmd)
    os.system("chmod 777 " + npu_info_dir)
    set_bashrc("root")

    ## 生成训练脚本
    generate_tensorflow()
    return


if __name__ == "__main__":


    if "aiframework" in os.environ:

        framework = os.environ["aiframework"].strip()

        if framework == "tensorflow":
            handle_tensorflow()

        elif framework == "mindspore":
            handle_mindspore()

        else:
            pass

    else:

        generate_mindspore()
        generate_tensorflow()

    pass
