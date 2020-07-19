#!/bin/bash

###################################################################################################
# @file bboxConf.sh                                                                               #
#                                                                                                 #
# Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.                     #
#                                                                                                 #
# This shell is bbox configuration shell script for set and get bbox configurations               #
###################################################################################################

BBOX_CFG_FILE="/var/log/npu/conf/bbox/bbox.conf"
DEFAULT_BBOX_LOGPATH="/var/log/npu/hisi_logs"

# get and check config file valid
function get_config_file_path(){
    if [ ! -f "$BBOX_CFG_FILE" ]; then
        # Can not find $BBOX_CFG_FILE 
        return 1;
    fi
    BBOX_CFG_FILE="/var/log/npu/conf/bbox/bbox.conf"
    return 0
}

# process query cmd options
function query_bbox_path(){
    local path=""
    local kword="MNTN_PATH="

    get_config_file_path

    if [ $? -eq 0 ] && [ -f "$BBOX_CFG_FILE" ]; then
        path=$(grep "$kword" "$BBOX_CFG_FILE" |grep -v "^#" |awk -F "$kword" '{print$NF}' |head -1)
    fi

    # Check bbox file path
    if [ -z "$path" ] || \
        [[ "$path" != /* ]] || \
        [[ "$path" == */../* ]] || \
        [[ "$path" == */.. ]]; then
        # check MNTN home path format failed, use default path
        path="$DEFAULT_BBOX_LOGPATH"
    fi

    if [ ! -d "$path" ]; then
        echo "Get bbox MNTN log path $path, but it is not a directory!"
        return 1
    fi

    echo -e "$path"
    return 0
}

# process query cmd
function query_cmd_proc(){
    local option="$1"

    if [ -z "$option" ]; then
        echo "Need a sub-option for query"
        help_cmd_proc
        return 1
    fi

    if [ "$option" == "--hostDir" ]; then
        query_bbox_path
        return $?
    else
        echo "Unknown query option $option"
        help_cmd_proc
        return 1
    fi
}

# process help cmd
function help_cmd_proc(){
    echo -e "\
Usage:  -hqs [SUB_OPTIONS] ...
Get or set bbox options in bbox configuration file.
        -h, --help      display this help and exit
        -q              query for options, see sub-options below:
                        --hostDir       query for current bbox log dir

Some examples:
    bash bboxConf.sh -q --hostDir"

    return 0
}

function main(){
    # Check parameters
    if [ $# -eq 0 ]; then
        help_cmd_proc
        exit 1
    fi

    # Check -sq input parameters
    if [ "$1" = "-q" ]; then
        query_cmd_proc "$2"
    elif [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
        help_cmd_proc
    else
        echo "Unknown option $1"
        help_cmd_proc
        exit 1
    fi

    exit $?
}

main "$@"
