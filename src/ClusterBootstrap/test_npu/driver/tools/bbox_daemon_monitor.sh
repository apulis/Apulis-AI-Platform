#!/bin/bash

###################################################################################################
# @file bbox_daemon_monitor.sh                                                                    #
#                                                                                                 #
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.                     #
#                                                                                                 #
# This shell is for daemon monitors to recover bbox configuration file                            #
###################################################################################################

BBOX_CFG_DIR="/var/log/npu/conf/bbox"
BBOX_REDIRECT_LOG="/var/log/npu/bbox/bbox_monitor_log.txt"

bbox_log(){
    echo "$1" >> "${BBOX_LOG}"
    echo "$1" >> ${BBOX_REDIRECT_LOG}
}

function recover_bbox_conf(){
    local HIAI_DIR="$1"
    local BBOX_LOG="$2"

    date > ${BBOX_REDIRECT_LOG}

    if [ -z "${BBOX_LOG}" ]; then
        echo "no log file set, exit directly." >> ${BBOX_REDIRECT_LOG}
        return 1
    fi

    if [ -z "${HIAI_DIR}" ]; then
        bbox_log "hiai path not set, skip bbox config recovery."
        return 1
    fi

    if [ ! -d ${BBOX_CFG_DIR} ]; then
        mkdir -p ${BBOX_CFG_DIR}
        if [ $? -ne 0 ]; then
            bbox_log "fail to create ${BBOX_CFG_DIR}"
            return 1;
        fi

        chmod -f 750 ${BBOX_CFG_DIR}
        if [ $? -ne 0 ]; then
            bbox_log "fail to chmod ${BBOX_CFG_DIR}"
            return 1;
        fi
        bbox_log "mkdir ${BBOX_CFG_DIR} success."
    fi

    if [ ! -f ${BBOX_CFG_DIR}/bbox.conf ]; then
        cp "${HIAI_DIR}/driver/tools/bbox.conf" ${BBOX_CFG_DIR}
        if [ $? -ne 0 ]; then
            bbox_log "fail to cp bbox.conf"
            return 1;
        fi

        chmod -f 640 ${BBOX_CFG_DIR}/bbox.conf
        if [ $? -ne 0 ]; then
            bbox_log "fail to chmod bbox.conf"
            return 1;
        fi
        bbox_log "${BBOX_CFG_DIR}/bbox.conf recovered."
    fi
    return 0;
}

