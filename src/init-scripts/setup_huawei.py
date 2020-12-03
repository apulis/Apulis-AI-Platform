# -*- coding: utf-8 -*-
"""
Created on Sun Oct 25 11:35:11 2020

@author: DELL
"""

from string import Template
import argparse
parser = argparse.ArgumentParser(description='gen run python shell script')
parser.add_argument('--type', help='shell script for tensorflow or mindspore')
parser.add_argument('--command', help='python script path with args')
parser.add_argument('--out', help='shell script output path')
args = parser.parse_args();

if(args.command.replace(" ", "") =="sleepinfinity"):
    args.command = "{{replace command here}}"

if(args.type == "tensorflow"):
    ids="IFS=',' read -ra VISIBLE_IDS <<< \"${VISIBLE_IDS}\"\necho \"NPU---${VISIBLE_IDS[@]}\"\n"
    env_path='''
# setting main path
MAIN_PATH=$(dirname $(readlink -f $0))\n
export JOB_ID=$RANDOM
export SOC_VERSION=Ascend910
export HCCL_CONNECT_TIMEOUT=200\n
export GLOBAL_LOG_LEVEL=3
export TF_CPP_MIN_LOG_LEVEL=3
export SLOG_PRINT_TO_STDOUT=0\n
# local variable
export RANK_SIZE=${#VISIBLE_IDS[@]}
export RANK_TABLE_FILE=/home/$DLWS_USER_NAME/.npu/$DLWS_JOB_ID/hccl_tf.json
export POD_NAME=${DLWS_JOB_ID}
export RANK_ID=${DLWS_JOB_ID}
SAVE_PATH=$MAIN_PATH/training\n
# training stage\n
for device_phy_id in "${VISIBLE_IDS[@]}"
do
export DEVICE_ID=$device_phy_id
export DEVICE_INDEX=$device_phy_id
TMP_PATH=$SAVE_PATH/D$RANK_ID
mkdir -p $TMP_PATH
cd $TMP_PATH
# {{start command}}
    '''
    script=Template('${command} &\n')
    end='cd -\n\ndone\n\nwait'
elif(args.type == "mindspore"):
    ids="IFS=',' read -ra VISIBLE_IDS <<< \"${VISIBLE_IDS}\"\necho \"NPU---${VISIBLE_IDS[@]}\"\n"
    env_path='''
# setting main path
MAIN_PATH=$(dirname $(readlink -f $0))\n
export JOB_ID=$RANDOM
export SOC_VERSION=Ascend910
export HCCL_CONNECT_TIMEOUT=200\n

# local variable
export RANK_SIZE=${#VISIBLE_IDS[@]}
export RANK_TABLE_FILE=/home/$DLWS_USER_NAME/.npu/$DLWS_JOB_ID/hccl_ms.json
SAVE_PATH=$MAIN_PATH/training\n
# training stage\n
for device_phy_id in $(seq 0 $[$RANK_SIZE-1])
do
export DEVICE_ID=$device_phy_id
export RANK_ID=$device_phy_id
echo "start training for rank $RANK_ID, device $DEVICE_ID"
TMP_PATH=$SAVE_PATH/D$RANK_ID
mkdir -p $TMP_PATH
cd $TMP_PATH
# {{start command}}
    '''
    script=Template('${command} &\n')
    end='cd -\n\ndone\n\nwait'
    
txt = ids+env_path+script.substitute(command=args.command)+end
print(txt)
with open(args.out,"w") as f:
    f.write(txt)
    

