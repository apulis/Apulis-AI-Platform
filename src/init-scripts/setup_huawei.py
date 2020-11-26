# -*- coding: utf-8 -*-
"""
Created on Sun Oct 25 11:35:11 2020

@author: DELL
"""

from string import Template
import argparse
parser = argparse.ArgumentParser(description='gen run python shell script')
parser.add_argument('--command', help='python script path with args')
parser.add_argument('--out', help='shell script output path')
args = parser.parse_args();

if(args.command.replace(" ", "") =="sleepinfinity"):
    args.command = "#input python script command here"

ids="IFS=',' read -ra VISIBLE_IDS <<< '${VISIBLE_IDS}'\necho 'NPU---${VISIBLE_IDS[@]}'\n"
env_path='# setting main path\nMAIN_PATH=$(dirname $(readlink -f $0))\n\n # set env\nexport PYTHONPATH=/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/opp/op_impl/built-in/ai_core/tbe:$PYTHONPATH\nexport LD_LIBRARY_PATH=/usr/lib/aarch64-linux-gnu/hdf5/serial:/usr/local/Ascend/add-ons:/home/HwHiAiUser/Ascend/nnae/latest/fwkacllib/lib64:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/arm64-linux/atc/lib64:$LD_LIBRARY_PATH\nexport TBE_IMPL_PATH=/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/opp/op_impl/built-in/ai_core/tbe\nexport PATH=$PATH:/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/fwkacllib/ccec_compiler/bin/\nexport ASCEND_OPP_PATH=/usr/local/Ascend/ascend-toolkit/latest/arm64-linux/opp\nexport SOC_VERSION=Ascend910\nexport HCCL_CONNECT_TIMEOUT=200\n\nexport GLOBAL_LOG_LEVEL=3\nexport TF_CPP_MIN_LOG_LEVEL=3\nexport SLOG_PRINT_TO_STDOUT=0\n\n# local variable\nRANK_SIZE=${#VISIBLE_IDS[@]}\nRANK_TABLE_FILE=/home/$DLWS_USER_NAME/.npu/$DLWS_JOB_ID/hccl_tf.json\nexport POD_NAME=${DLWS_JOB_ID}\nexport RANK_ID=${DLWS_JOB_ID}\nSAVE_PATH=$MAIN_PATH/training\n\n# training stage\n\nfor device_phy_id in "${VISIBLE_IDS[@]}"\ndo\nexport DEVICE_ID=$devcice_phy_id\nexport DEVICE_INDEX=$devcice_phy_id\nTMP_PATH=$SAVE_PATH/D$RANK_ID\nmkdir -p $TMP_PATH\ncd $TMP_PATH\n'
script=Template('sudo -E bash ${command} &\n')
end='cd -\n\ndone\n\nwait'
txt = ids+env_path+script.substitute(command=args.command)+end
print(txt)
with open(args.out,"w") as f:
    f.write(txt)
    
