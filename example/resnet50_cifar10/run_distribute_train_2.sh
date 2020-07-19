#!/bin/bash
# Copyright 2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

if [ $# != 2 ]
then
    echo "Usage: sh run_distribute_train.sh [MINDSPORE_HCCL_CONFIG_PATH] [DATASET_PATH]"
exit 1
fi

if [ ! -f $1 ]
then
    echo "error: MINDSPORE_HCCL_CONFIG_PATH=$1 is not a file"
exit 1
fi

if [ ! -d $2 ]
then
    echo "error: DATASET_PATH=$2 is not a directory"
exit 1
fi

#sudo ulimit -u unlimited
ulimit unlimited
export DEVICE_NUM=8
export RANK_SIZE=16
export MINDSPORE_HCCL_CONFIG_PATH=$1

export LD_LIBRARY_PATH=/usr/local/lib/:/usr/lib/:/usr/local/Ascend/fwkacllib/lib64/:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/usr/local/Ascend/add-ons/
export PYTHONPATH=$PYTHONPATH:/usr/local/Ascend/opp/op_impl/built-in/ai_core/tbe:/code
export PATH=$PATH:/usr/local/Ascend/fwkacllib/ccec_compiler/bin
export ASCEND_OPP_PATH=/usr/local/Ascend/opp
export SOC_VERSION=ASCEND910
#export DDK_VERSION_FLAG=1.60.T17.B830
export HCCL_CONNECT_TIMEOUT=200

for((i=0; i<${DEVICE_NUM}; i++))
do
    export DEVICE_ID=$i
    export RANK_ID=$(($i+8))
    rm -rf ./train_parallel$(($i+8))
    mkdir ./train_parallel$(($i+8))
    cp *.py ./train_parallel$(($i+8))
    cp *.sh ./train_parallel$(($i+8))
    cd ./train_parallel$(($i+8)) || exit
    echo "start training for rank $RANK_ID, device $DEVICE_ID"
    #env > env.log
    python3.7 train.py --do_train=True --run_distribute=True --device_num=$DEVICE_NUM --dataset_path=$2 &
    cd ..
done

trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT
wait
