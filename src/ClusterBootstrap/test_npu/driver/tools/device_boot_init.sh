#!/bin/bash

# Copyright © Huawei Technologies Co., Ltd. 2010-2020. All rights reserved.
# 1. raised up by ascend+_monitor process when device boot.
# 2. upgrade aicpu packet to device.
# 3. restore network configuration.
# 4. this script is raised by user root, so can't not open other scripts here.
#    because if using other script here, they also has root authority,this takes 
#    secuity risk.
set -e

install_info="/etc/ascend_install.info"
. $install_info
install_path=$Driver_Install_Path_Param
aicpu_kernels_run=$(find ${install_path}/ -name *aicpu_kernels.tar.gz)
log_file="/var/log/device_boot_init.log"
upgrade_result_file="aicpu_upgrade_result_tmp.log"

log() {
        if [ ! -f "$log_file" ]; then
                touch $log_file
                chmod 600 $log_file
        fi
        chmod 600 $log_file
        cur_date=$(date +"%Y-%m-%d %H:%M:%S")
        echo "[aicpu] [$cur_date] "$1 >> $log_file
}

aicpu_upgrade() {
        local num=0
        local count=30
        local upgrade_flag=$((${3}%2))

        # check upgrade-tool and aicpu file
        if [ ! -f "${aicpu_kernels_run}" ]; then
                log "aicpu_upgrade error:aicpu kernels run path is not exist"
                return 0
        fi

        if [ ! -f "${install_path}"/driver/tools/upgrade-tool ]; then
                log "aicpu_upgrade error: ${install_path}/opp/aicpu/upgrade_${1}/upgrade-tool is not exist"
                return 1
        fi

        # check if is need to upgrade 
        if [ "${upgrade_flag}" -eq 0 ]; then
                log "device_${1} aicpu_upgrade success, upgrade flag is 0."
                if [ -f "${install_path}"/opp/aicpu/"${upgrade_result_file}" ]; then
                        echo "device_${1} aicpu_upgrade success." >> ${install_path}/opp/aicpu/${upgrade_result_file}
                fi
                return 0
        fi

        # creat dir for upgrade tmp file
        rm -Rf /var/driver/upgrade_${1}
        mkdir -p /var/driver/upgrade_${1}
        chmod 500 /var/driver/upgrade_${1}
        cd /var/driver/upgrade_${1}/
        touch aicpu_upgrade_status.log
        chmod 600 aicpu_upgrade_status.log

        while ((${count}))
        do
            let count=${count}-1
            ${install_path}/driver/tools/upgrade-tool --device_index ${1} --components >aicpu_upgrade_status.log 2>&1 &
            sleep 1

            num=$(echo $(grep '{"device_id":' aicpu_upgrade_status.log | grep -c 'aicpu'))
            if [ "${num}" -eq 1 ]; then
                    break
            else
                    log "device_${1} upgrade-tool get component failed."
            fi
        done
        if [ "${num}" -eq 0 ]; then
                log "device_${1} upgrade-tool error!"
                rm -rf /var/driver/upgrade_${1}
                return 1
        fi
        num=0

        touch aicpu_upgrade_tmp.log
        chmod 600 aicpu_upgrade_tmp.log
        ${install_path}/driver/tools/upgrade-tool --device_index ${1} --component aicpu --path ${aicpu_kernels_run} >aicpu_upgrade_tmp.log 2>&1 &
        count=60
        while ((${count}))
        do
            sleep 3
            num=$(echo $(grep '{"device":' aicpu_upgrade_tmp.log | grep -c 'succeed'))
            if [ "${num}" -eq 1 ]; then
                    log "device_${1} aicpu_upgrade success."
                    if [ -f "${install_path}"/opp/aicpu/"${upgrade_result_file}" ]; then
                            echo "device_${1} aicpu_upgrade success." >> ${install_path}/opp/aicpu/${upgrade_result_file}
                    fi
                    break
            fi
            num=$(echo $(grep '{"device":' aicpu_upgrade_tmp.log | grep -c 'failed'))
            if [ "${num}" -eq 1 ]; then
                    log "device_${1} aicpu_upgrade failed!"
                    if [ -f "${install_path}"/opp/aicpu/"${upgrade_result_file}" ]; then
                            echo "device_${1} aicpu_upgrade failed!" >> ${install_path}/opp/aicpu/${upgrade_result_file}
                    fi
                    log "device_${1} error: $(cat aicpu_upgrade_tmp.log)"
                    break
            fi

            log "device_${1} waiting for upgrade result ..."
            let count=${count}-1
        done
        if [ "${count}" -eq 0 ]; then
            log "device_${1} aicpu upgrade fail, no result log."
        fi

        rm -rf /var/driver/upgrade_${1}
}

network_cfg_recovery() {
        log "device_${1} slot_id is ${2}, hccn_tool recovery"
        nohup ${install_path}/driver/tools/hccn_tool -i ${2} -cfg recovery > /dev/null 2>&1 &
}

log "<<<<<<<< device_${1} boot init >>>>>>>>"
log "device boot init start, logic id: $1, phyical id: $2, upgrade flag: $3, ip_addr: 127.0.0.1, user: root"

# restore network cfg
log "<<<<<<<< device_${1} network cfg recovery start >>>>>>>>"
network_cfg_recovery ${1} ${2} ${3}
log "<<<<<<<< device_${1} network cfg recovery end >>>>>>>>"

# upgrade aicpu
log "<<<<<<<< device_${1} aicpu upgrade start >>>>>>>>"
aicpu_upgrade ${1} ${2} ${3}
log "<<<<<<<< device_${1} aicpu upgrade end!! >>>>>>>>"

