#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import json
import time
import pdb
import platform
import string
import random


# 此脚本与create_script.sh由算法同事
# 帮忙维护，当代码变更时需更新此版本号
code_version="1.0"


def create_hccl_mindspore():

    done = 0
    rank_id = 0
    hccl_data = {}

    # for test only
    #os.environ['DLWS_WORKER_NUM'] = "2"
    #os.environ['DLWS_JOB_ID'] = "test_npu_device"
    #os.environ['DLWS_USER_NAME'] = "bifeng.peng"
    #

    ## 单机任务，用DLWS_PS_NUM=0判断最好
    if "DLWS_WORKER_NUM" not in os.environ:
        os.environ['DLWS_WORKER_NUM'] = "1"
    else:
        pass

    worker_num = int(os.environ['DLWS_WORKER_NUM'])
    job_id = os.environ['DLWS_JOB_ID']
    user_name =  os.environ['DLWS_USER_NAME']

    # 1）hccl文件和相关脚本都会放到此目录
    # 2）文件和具体的JOB有关, 不同JOB隔离存储
    npu_dir = '/home/%s/.npu/%s/' % (user_name, job_id)

    # 以下变量写死
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

    ## 生成npu_idx.info文件
    ## 文件数量和worker个数一致
    while True:

        PATH = npu_dir + ('/npu_%d.info' % (done))
        if os.path.isfile(PATH) and os.access(PATH, os.R_OK):

            
            with open(PATH, "r") as f:

                ips = ""
                host_ip = ""

                # 文件中的格式：
                # ip=id1:ip1,id2:ip2
                # host=xxx
                for line in f:
                    print(line)
                    if "ip=" in line:
                        _, ips = line.strip().split("=")
                    elif "host=" in line:
                        _, host_ip = line.strip().split("=")

                ip_list = ips.split(",")
                ip_list = sorted(ip_list)

                for ip_elem in ip_list:

                    # 设备id和ip
                    device_id, device_ip = ip_elem.split(":")

                    ## set up group list
                    device_item = {}  # item of instance list
                    device_item["devices"] = [{
                        "device_id" : device_id,
                        "device_ip" : device_ip
                    }]

                    device_item["rank_id"] = str(rank_id)
                    device_item["server_id"] = str(host_ip)

                    #pdb.set_trace()
                    rank_id = rank_id + 1
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

def create_hccl_tensorflow():

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

    distributing_job= False
    if "DLWS_NUM_PS" in os.environ:
        if int(os.environ["DLWS_NUM_PS"]) > 0: 
            distributing_job = True
        else:
            pass
    else:
        pass
    

    # 1）hccl文件和相关脚本都会放到此目录
    # 2）文件和具体的JOB有关, 不同JOB隔离存储
    npu_dir = '/home/%s/.npu/%s/' % (user_name, job_id)

    hccl_data["group_count"] = "1"
    hccl_data["status"] = "completed"
    hccl_data["group_list"] = []

    group = {}
    #group["device_count"] = worker_num * 8
    group["instance_count"] = str(worker_num)
    group["group_name"] = "test"
    group["instance_list"] = []

    ## 生成npu_idx.info文件
    ## 文件数量和worker个数一致
    while True:

        PATH = npu_dir + ('/npu_%d.info' % (done))
        if os.path.isfile(PATH) and os.access(PATH, os.R_OK):

            with open(PATH, "r") as f:

                ips = ""
                host_ip = ""

                # 文件中的格式：
                # ip=id1:ip1,id2:ip2
                # host=xxx
                for line in f:
                    print(line)
                    if "ip=" in line:
                        _, ips = line.strip().split("=")
                    elif "host=" in line:
                        _, host_ip = line.strip().split("=")

                instance_item = {}  # item of instance list
                if distributing_job is True:
                    instance_item["pod_name"] = job_id + "-worker-" + str(done)
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


# 从/pod.env导入环境变量
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

# 向/pod.env写入环境变量
# 先判断是否存在此环境量，如果已存在，则覆盖
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

    osflag="x86_64"

    if platform.machine() == "aarch64":
        osflag = "arm64"
    else:
        pass

    return osflag

# gnu安装目录中的架构和算法组件的不一样
# 单独处理
def get_gnu_arch_flag():

    osflag="x86_64"

    if platform.machine() == "aarch64":
        osflag = "aarch64"
    else:
        pass

    return osflag


def get_random_num(length):
    return ''.join(random.choice(string.digits) for _ in range(length))


# 用于将环境变量更新 写入指定用户的shell加载文件
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


# 准备mindspore环境
# 1) 预备环境变量，并写入/pod.env
# 2) 创建算法需要的训练shell脚本
# 3) 创建算法需要的hccl文件
def handle_mindspore():

    path = "/pod.env"
    envs = load_env(path)  # 导入平台加载过程中已创建的环境变量
    envs_to_add= {}
    envs_to_add["DEVICE_ID"] = "0"

    # 解析GPU/NPU设备ID
    if "VISIBLE_IDS" in envs:
        envs["VISIBLE_IDS"] = envs["VISIBLE_IDS"].replace("\\","")
        envs_to_add["VISIBLE_IDS"] = envs["VISIBLE_IDS"] 
    else:
        pass


    # 解析NPU Device ID
    if "NPU_IPS" in envs:
        envs["NPU_IPS"] = envs["NPU_IPS"].replace("\\","")
        envs_to_add["NPU_IPS"] = envs["NPU_IPS"] 
    else:
        pass

    ## 将/pod.env已有的环境变量
    ## 与os当前具有的环境变量合并, 放入envs
    for k, v in os.environ.items():
        if k not in envs:
            envs[k] = v
        else:
            pass

    ## 不需要解析device id

    ## 设置随机参数, 算法要求
    envs["RANDOM"] = get_random_num(6)
    envs["osflag"] = get_os_flag()
    envs["gnu_arch"] = get_gnu_arch_flag()

    # mindspore环境变量模板
    mindspore_envs = [
      "PYTHONPATH=/usr/local/lib/python3.7/site-packages/mindspore/lib:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:${PYTHONPATH}",
      "LD_LIBRARY_PATH=/usr/lib/${gnu_arch}-linux-gnu/hdf5/serial:/usr/local/Ascend/add-ons/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/fwkacllib/lib64:/usr/local/Ascend/add-ons:/home/HwHiAiUser/Ascend/nnae/latest/fwkacllib/lib64:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/opp/op_impl/built-in/ai_core/tbe/op_tiling:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/atc/lib64:/usr/local/Ascend/fwkacllib/lib64/:/usr/local/lib/python3.7/site-packages/mindspore/lib/:/usr/local/lib:/home/clang+llvm/lib/:$LD_LIBRARY_PATH",
      "TBE_IMPL_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:/usr/local/Ascend/ascend-toolkit/latest/opp/op_impl/built-in/ai_core/tbe",
      "PATH=$PATH:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/fwkacllib/ccec_compiler/bin/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/fwkacllib/ccec_compiler/bin/:/home/clang+llvm/bin/",
      "ASCEND_OPP_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/opp",
      "LLVM_CONFIG=/home/clang+llvm/bin/llvm-config",
      "SOC_VERSION=Ascend910",
      "POD_NAME=${DLWS_JOB_ID}",
      "JOB_ID=${RANDOM}",
      "RANK_SIZE=1",
      "ASCEND_GLOBAL_LOG_LEVEL=3",
      "ASCEND_GLOBAL_EVENT_ENABLE=0"
    ]

    # 模板渲染
    for item in mindspore_envs:

        tpl = string.Template(item)
        new_item = tpl.safe_substitute(envs)

        if "=" in new_item:
            key_val = new_item.strip().split("=")
            k = key_val[0]
            v = key_val[1]
            envs_to_add[k] = v

        else:
            pass

    # 1) 更新/pod.env, 创建环境变量
    add_env(path, envs_to_add)

    # 2) 生成shell训练脚本
    pod_cmd = os.environ["DLWS_LAUNCH_CMD"]
    npu_info_dir = "/home/" + os.environ["DLWS_USER_NAME"] + "/.npu/" + os.environ["DLWS_JOB_ID"] + "/train.sh"

    cmd = 'python /pod/scripts/create_script.py --type mindspore --command "%s" --out %s'% (pod_cmd, npu_info_dir)
    os.system(cmd)
    os.system("chmod 777 " + npu_info_dir)

    # 将环境变量更新写入 root
    set_bashrc("root")

    ## 3) 生成hccl_tf.json
    if need_create_hccl() is True:
        create_hccl_mindspore()
    else:
        pass

    # 4) 分布式训练任务，环境配置同步
    if is_distributed_job() is True and is_ps_pod() is True:
        notify()

    elif is_distributed_job() is True and is_worker_pod() is True:
        wait()

    else:
        pass

    return


# 准备tensorflow环境
# 1) 预备环境变量，并写入/pod.env
# 2) 创建算法需要的训练shell脚本
# 3) 创建算法需要的hccl文件
def handle_tensorflow():

    # 1) 预备环境变量，并写入/pod.env
    path = "/pod.env"
    envs = load_env(path) # 导入平台加载过程中已创建的环境变量
    envs_to_add= {}

    # 解析GPU/NPU设备ID
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

    ## 将/pod.env已有的环境变量
    ## 与os当前具有的环境变量合并, 放入envs
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

    # 模板配置
    tensorflow_envs = [
      "PYTHONPATH=/usr/local/lib/python3.7/site-packages/mindspore/lib:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:${PYTHONPATH}",
      "LD_LIBRARY_PATH=/usr/lib/${gnu_arch}-linux-gnu/hdf5/serial:/usr/local/Ascend/add-ons/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/fwkacllib/lib64:/usr/local/Ascend/add-ons:/home/HwHiAiUser/Ascend/nnae/latest/fwkacllib/lib64:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/opp/op_impl/built-in/ai_core/tbe/op_tiling:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/atc/lib64:/usr/local/Ascend/fwkacllib/lib64/:/usr/local/lib/python3.7/site-packages/mindspore/lib/:/usr/local/lib:/home/clang+llvm/lib/:$LD_LIBRARY_PATH",
      "TBE_IMPL_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:/usr/local/Ascend/ascend-toolkit/latest/opp/op_impl/built-in/ai_core/tbe",
      "PATH=$PATH:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/fwkacllib/ccec_compiler/bin/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/fwkacllib/ccec_compiler/bin/:/home/clang+llvm/bin/",
      "ASCEND_OPP_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/opp",
      "LLVM_CONFIG=/home/clang+llvm/bin/llvm-config",
      "SOC_VERSION=Ascend910",
      "POD_NAME=${DLWS_JOB_ID}",
      "JOB_ID=${RANDOM}",
      "RANK_SIZE=1",
      "ASCEND_GLOBAL_LOG_LEVEL=3",
      "ASCEND_GLOBAL_EVENT_ENABLE=0"
    ]

    envs_to_add["DEVICE_ID"] = device_id
    envs_to_add["DEVICE_INDEX"] = device_index

    # 渲染模板
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

    #  1) 更新环境变量
    add_env(path, envs_to_add)

    ## 2) 生成shell脚本
    pod_cmd = os.environ["DLWS_LAUNCH_CMD"]
    npu_info_dir = "/home/" + os.environ["DLWS_USER_NAME"] + "/.npu/" + os.environ["DLWS_JOB_ID"] + "/train.sh"

    cmd = 'python /pod/scripts/create_script.py --type tensorflow --command "%s" --out %s'% (pod_cmd, npu_info_dir)
    print(cmd, "==========================")
    os.system(cmd)
    os.system("chmod 777 " + npu_info_dir)

    # 更新用户bash脚本
    set_bashrc("root")

    # 3) 生成hccl_tf.json
    if need_create_hccl() is True:
        create_hccl_tensorflow()
    else:
        pass

    # 4) 分布式训练任务，环境配置同步
    if is_distributed_job() is True and is_ps_pod() is True:
        notify()

    elif is_distributed_job() is True and is_worker_pod() is True:
        wait()

    else:
        pass


    return

# 是否分布式训练任务
def is_distributed_job():

    if "DLWS_NUM_PS" in os.environ:
        dlws_num_ps = os.environ["DLWS_NUM_PS"].strip().lower()

        if len(dlws_num_ps) > 0 and int(dlws_num_ps) >0:
            print("is_distributed_job return true")
            return True

    return False

# 是否master节点
def is_ps_pod():

    if "DLWS_ROLE_NAME" in os.environ:
        dlws_role_name = os.environ["DLWS_ROLE_NAME"].strip().lower()

        ## Ps表示多机多卡ps pod
        if dlws_role_name == "ps":
            return True

    return False


# 是否worker节点
def is_worker_pod():

    if "DLWS_ROLE_NAME" in os.environ:
        dlws_role_name = os.environ["DLWS_ROLE_NAME"].strip().lower()

        ## Ps表示多机多卡ps pod
        if dlws_role_name == "worker":
            return True

    return False


# 分布式训练任务 
# ps节点在环境预备结束后，创建setup_environment_done文件
# 用作环境准备完成的标识
def notify():

    # 单机训练任务，只有一个POD不需要做协同
    if is_distributed_job() is False:
        return

    setup_environment_done = "/home/" + os.environ["DLWS_USER_NAME"] + "/.npu/" + os.environ["DLWS_JOB_ID"] + "/setup_environment_done"

    # 多机多卡训练，ps节点预备环境
    if not os.path.exists(setup_environment_done):
        open(setup_environment_done, 'a').close()

    return

# 分布式训练任务 
# worker节点通过检查setup_environment_done文件
# 来判断环境准备是否结束
def wait():

    # 单机训练任务，只有一个POD不需要等待环境
    if is_distributed_job() is False:
        return

    setup_environment_done = "/home/" + os.environ["DLWS_USER_NAME"] + "/.npu/" + os.environ["DLWS_JOB_ID"] + "/setup_environment_done"

    # 多机多卡训练，ps节点预备环境
    while True:
        if not os.path.exists(setup_environment_done):
            print("===========", setup_environment_done, " not found. wait")
            time.sleep(1)
        else:
            break

    return



# 1) 单机训练中，需要创建hccl文件
# 2）多机多卡中，需要在ps pod创建hccl文件, 此文件会被worker pod共同读取
def need_create_hccl():
    

    if "DLWS_ROLE_NAME" in os.environ:
        dlws_role_name = os.environ["DLWS_ROLE_NAME"].strip().lower()

        ## master表示单机POD
        ## Ps表示多机多卡ps pod
        if dlws_role_name == "ps" or dlws_role_name == "master":
            return True

    return False


if __name__ == "__main__":

    # 1) 训练框架类别由前端传入
    #    本脚本依据此字段, 为不同框架创建不同的环境参数
    #    hccl文件、环境变量等等

    # 2) 脚本经平台bootstrap.sh调用
    #    仅在JOB为单机节点或者 分布式任务的PS节点被执行
    if "aiframework" in os.environ:

        framework = os.environ["aiframework"].strip().lower()

        if framework == "tensorflow":
            handle_tensorflow()

        elif framework == "mindspore":
            handle_mindspore()

        else:
            pass

    else:

        # 兼容版本<v1.3.0
        create_hccl_mindspore()
        create_hccl_tensorflow()

    pass
