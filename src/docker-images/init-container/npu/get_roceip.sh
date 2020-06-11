#!/bin/bash
#export PYTHONPATH=$PYTHONPATH:/usr/local/HiAI/runtime/ops/op_impl/built-in/ai_core/tbe
#export LD_LIBRARY_PATH=/usr/local/lib/:/usr/local/python3/lib:/usr/lib/:/usr/local/HiAI/runtime/lib64/:/usr/local/HiAI/driver/lib64/
#export DDK_VERSION_PATH=/usr/local/HiAI/runtime/ddk_info
#export PATH=/usr/local/HiAI/runtime/ccec_compiler/bin:$PATH

device_id=$1

currentDir=$(cd "$(dirname "$0")"; pwd)
cd ${currentDir}

ostype=`uname -m`
if [ x"${ostype}" = xaarch64 ];
then
    GET_DEVICE_IP=`${currentDir}/turing_dsmi_arm func_dsmi_get_device_ip_address 1 ${device_id} 1 ${device_id}`
else
    GET_DEVICE_IP=`${currentDir}/turing_dsmi_x86 func_dsmi_get_device_ip_address 1 ${device_id} 1 ${device_id}`
fi

DEVICE_IP=`echo ${GET_DEVICE_IP} | grep "device_ip =" | awk -F " " '{print $(NF-3)}'`

echo "ROCE_IP=${DEVICE_IP}"
