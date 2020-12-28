#!/bin/bash

#需要采集哪个device的性能数据
devId=$1
if [ -z "${devId}" ]; then
    echo "[ERROR] Please input devId"
    exit 1
fi

if [[ ! ${devId} =~ ^([0-7])$ ]]; then
    echo "[ERROR] Please input valid devId:${devId}"
    exit 1
fi

#GE Options
#按需配置为true或者false
export PROFILING_MODE=true
#按需配置，此处配置的是task粒度的性能数据+迭代轨迹的5个点信息
export PROFILING_OPTIONS=training_trace:task_trace
#按需配置，此处配置的是resnet网络的FP Start打点信息
export FP_POINT=fp32_vars/conv2d/Conv2Dfp32_vars/BatchNorm/FusedBatchNorm_Reduce
#按需配置，此处配置的是resnet网络的BP End打点信息
export BP_POINT=loss_scale/gradients/AddN_70

#DATA PROCESS  Options
#按需配置为true或者false，此处配置的是不采集数据增强模块的性能数据
export AICPU_PROFILING_MODE=false

#PROFILING  Options
inotifiDir=/var/log/npu/profiling/container/${devId}
if [ ! -d "${inotifiDir}" ]; then
    mkdir "${inotifiDir}"
    if [ $? -ne 0 ]; then
        echo "[ERROR] Create profiling_dir:${inotifiDir} error!"
        exit 1
    fi
    sleep 1
fi
export PROFILING_DIR=${inotifiDir}
exit 0