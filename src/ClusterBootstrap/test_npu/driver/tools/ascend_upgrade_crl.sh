#!/bin/bash

# Copyright © Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
# 1. upgrade crl file 
# 2. when image cms has been revoked, need to indicate user to upgrade image file
set -e

CRL_FILE="$1"
CHECK_TOOL_PATH="/root/ascend_check"
INSTALL_INFO="/etc/ascend_install.info"
CHECK_TOOL_PATH_CRL="$CHECK_TOOL_PATH/ascend_910.crl"
CHECK_TOOL_FILE="$CHECK_TOOL_PATH/ascend_910_check.bin"
CRL_TMP_FILE="$CHECK_TOOL_PATH/ascend_910_tmp.crl"
LOG_FILE="/var/log/ascend_crl_upgrade.log"

log() {
    if [ ! -f "$LOG_FILE" ];then
        touch $LOG_FILE
        chmod 600 $LOG_FILE
    fi

    chmod 600 $LOG_FILE
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[crl upgrade] [$cur_date] "$1 >> $LOG_FILE
}

# check upgrade crl's integrity
check_crl_integrity() {
    local result=0
    if [ ! -f "$CRL_FILE" ];then
        log "[ERROR] crl file $CRL_FILE is not exist, please input valid file name"
        echo "upgrade fail, input crl file path is illegal, find crl file fail."
        return 1
    fi

    if [ ! -f "$CHECK_TOOL_FILE" ]; then
        log "[ERROR] crl check tool is not exist."
        echo "upgrade fail, crl check tool is not exist."
        return 1
    fi

    # use crl check tool to get crl file is valid or not, para 2 means check integrity
    result=$(echo "$($CHECK_TOOL_FILE 2 $CRL_FILE)$?")
    if [ "$result" -ne 0 ]; then
        log "[ERROR] input crl file is not a valid file, ret=$ret."
        echo "upgrade fail, input crl file is illegal, please choose legal file to upgrade."
        return 1
    else
        log "[INFO] crl file integrity check pass."    
    fi
}

# update crl file
crl_file_update() {
    if [ ! -d "$CHECK_TOOL_PATH" ]; then 
        mkdir -p $CHECK_TOOL_PATH
        chmod 500 $CHECK_TOOL_PATH
        log "[INFO] mkdir $CHECK_TOOL_PATh success"
    fi

    if [ ! -e "$CHECK_TOOL_PATH_CRL" ]; then
        touch $CHECK_TOOL_PATH_CRL
        chmod 400 $CHECK_TOOL_PATH_CRL
        log "[INFO] touch ascend_910.crl success"
    fi

    cp -f $CRL_FILE $CHECK_TOOL_PATH_CRL
}

# check check
upgrade_crl() {
    local IMG_FILE=0
    local DRV_INSTALL_PATH=0
    local result=0

    log "[INFO] start to upgrade crl file, file_path: $CRL_FILE, ip_addr: 127.0.0.1, user: root"

    # check upgrade crl file's integrity, if not valid, return fail
    check_crl_integrity
    if [ "$?" -ne 0 ];then 
        log "[ERROR] check crl file intgrity fail"
        return 1
    fi

    # check upgrade crl file and old crl file, if upgrade crl is old, return fail
    if [ -e "$CHECK_TOOL_PATH_CRL" ]; then 
        log "[INFO] old crl file is exist"
        result=$(echo "$($CHECK_TOOL_FILE 2 $CHECK_TOOL_PATH_CRL)$?") 
        if [ "$result" -eq 0 ]; then
            result=$(echo "$($CHECK_TOOL_FILE 1 $CRL_FILE $CHECK_TOOL_PATH_CRL)$?")
            if [ "$result" -gt 1 ]; then
                log "[ERROR] upgrade crl file is old, upgrade failed"
                echo "upgrade fail, upgrade crl file is old, please choose legal file to upgrade."
                return 1
            fi
        else
            log "[ERROR] $CHECK_TOOL_PATH_CRL is illegal."
            echo "upgrade fail, $CHECK_TOOL_PATH_CRL is illegal, please delete this file first."
            return 1
        fi
    else
        log "[INFO] $CHECK_TOOL_PATH_CRL is not exist."
    fi

    # parse image crl content and compare
    . $INSTALL_INFO
    DRV_INSTALL_PATH=$Driver_Install_Path_Param
    IMG_PATH="$DRV_INSTALL_PATH/driver/device"
    IMG_FILE="$IMG_PATH/ascend_910_tee.bin"
    if [ -f "$IMG_FILE" ]; then
        touch $CRL_TMP_FILE
        chmod 600 $CRL_TMP_FILE
         
        # get img crl content and save to crl temp file
        result=$(echo "$($CHECK_TOOL_FILE 0 $IMG_FILE $CRL_TMP_FILE)$?")
        if [ "$result" -ne 0 ]; then
            log "[INFO] image do not have crl file, upgrade crl file directly"
        else 
            result=$(echo "$($CHECK_TOOL_FILE 3 $CRL_FILE $IMG_FILE)$?")
            if [ $result -ne 0 ]; then
                rm -f $CRL_TMP_FILE
                log "[ERROR] image verify failed, need to upgrade image file first"
                echo "[ERROR] image file is illegal, please upgrade driver run package first."
                return 1 
            fi

            log "[INFO] image verify success"
        fi

        rm -f $CRL_TMP_FILE
    else
        log "[INFO] $IMG_FILE is not exist."
    fi

    crl_file_update    
    log "[INFO] upgrade crl file success"
    echo "upgrade crl success."
    return 0
}

# upgrade process 
upgrade_crl
