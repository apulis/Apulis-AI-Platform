#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
pci_dev_path="/sys/bus/pci/devices/"
logFile="${ASCEND_SECLOG}/ascend_install.log"
hotreset_status_file="/var/log/hotreset_status.log"

log() {
        cur_date=`date +"%Y-%m-%d %H:%M:%S"`
        echo "[driver] [$cur_date] "$1 >> $logFile
}

check_user_process()
{
    local dev_files="davinci0 davinci1 davinci2 davinci3 davinci4 davinci5 davinci6 davinci7 davinci_manager devmm_svm hisi_hdc"
    local f_file
    local apps
    local app
        local docker_dev
        local docker_list
    local ret=0

    for f_file in $dev_files
        do
        apps=`fuser -uv /dev/"$f_file"`
        if [ -n "${apps}" ]; then
            log $f_file" has user process: ""$apps"
            for app in $apps
            do
                ps -ef| grep $app| grep -v grep >> $logFile
            done
            ret=1
        fi
                docker_list=`docker ps -q`
                if [ -n "${docker_list}" ]; then
                        docker_dev=`docker inspect $(docker ps -q)| grep "$f_file"`
                        if [ -n "${docker_dev}" ]; then
                                log $f_file" has used by docker"
                                ret=1
                        fi
                fi
        done
        return $ret
}

os_name() {
    if [ ! -f $1 ];then
        HostOsName=unknown
        HostOsVersion=unknown
        return
    fi

    # Ubuntu
    cat $1 | awk 'NR==1{print $1}' | grep -i Ubuntu >>/dev/null
    if [ $? -eq 0 ];then
        HostOsName=Ubuntu
        HostOsVersion=$(cat $1 | grep VERSION_ID | awk -F "\"" '{print $2}')
        return
    fi

    # Debian
    cat $1 | awk 'NR==1{print $1}' | grep -i Debian >>/dev/null
    if [ $? -eq 0 ];then
        HostOsName=Ubuntu
        HostOsVersion=$(cat $1 | grep VERSION_ID | awk -F "\"" '{print $2}')
        return
    fi


    # CentOS
    cat $1 | awk 'NR==1{print $1}' | grep -i CentOS >>/dev/null
    if [ $? -eq 0 ];then
        HostOsName=CentOS
        HostOsVersion=$(cat $1 | grep VERSION_ID | awk -F "\"" '{print $2}')
        return
    fi

    # EulerOS
    cat $1 | awk 'NR==1{print $1}' | grep -i EulerOS >>/dev/null
    if [ $? -eq 0 ];then
        HostOsName=EulerOS
        HostOsVersion=$(cat $1 | grep VERSION_ID | awk -F "\"" '{print $2}')
        return
    fi

    # SuSE
    cat $1 | grep -i SuSE >>/dev/null
    if [ $? -eq 0 ];then
        HostOsName=SuSE
        HostOsVersion=unknown
        return
    fi

    # redhat
    cat $1 | grep -i redhat >>/dev/null
    if [ $? -eq 0 ];then
        HostOsName=redhat
        HostOsVersion=unknown
        return
    fi

    HostOsName=unknown
    HostOsVersion=unknown
    return
}


check_euler_os_process()
{
    HostOsName=unknown
    HostOsVersion=unknown

    if [ -f /etc/os-release ];then
        os_name /etc/os-release
    elif [ -f /etc/centos-release ];then
        HostOsName=CentOS
        HostOsVersion=$(cat /etc/centos-release | awk '{print $4}')
    else
        which lsb_release >/dev/null 2>&1
        if [ $? -eq 0 ];then
            HostOsName=$(lsb_release -si)
            HostOsVersion=$(lsb_release -sr)
        else
            os_name /etc/issue
        fi
    fi

    log "HostOsName : $HostOsName, HostOsVersion : $HostOsVersion"
    if [ "$HostOsName" = "EulerOS" ];then
        return 0
    fi
    return 1
}


kernel_version_lt()
{
    test "$(echo "$@" | tr " " "\n" | sort -rV | head -n 1)" != "$1";
}
check_kernel_version()
{
    cur_version=`uname -r | awk -F [-+] '{print $1}'`
    if kernel_version_lt $cur_version "4.7.0"; then
        echo "$cur_version"
        return 1
    fi

    return 0
}

remove_all_ko()
{
    local modules="drv_devdrv_host drv_pcie_vnic_host drv_pcie_hdc_host  drv_devmm_host drv_devmng_host drv_pcie_host "
    local module

    for module in $modules
    do
        log "[INFO]rmmod $module"
        rmmod $module
        if [ $? -ne 0 ];then
            log "[ERROR]rmmod failed"
            return 1
        fi
    done
    return 0
}
master_set_hotreset_flag()
{
        local bdfs=$1
        local bdf
        for bdf in $bdfs
        do
                log "[INFO]set hot reset flag: "$bdf
                echo 1 > $pci_dev_path$bdf"/hotreset_flag"
        done
}
get_bdfs()
{
    devnum1=`lspci -D -d 19e5:d801| awk '{print $1}'`
    local find_type=$1
    local bdf bdfs

    bdfs=""
    for bdf in $devnum1
    do
        if [ "$find_type" == "all" ];then
            bdfs=$bdfs$bdf" "
        fi
        if [ "$find_type" == "master" ];then
            if [ -e $pci_dev_path$bdf"/chip_id" ];then
                chip_id=`cat $pci_dev_path$bdf"/chip_id"`
                if [ "$chip_id" == "0" ];then
                    bdfs=$bdfs$bdf" "
                fi
            fi
                fi
        if [ "$find_type" == "slave" ];then
                        if [ -e $pci_dev_path$bdf"/chip_id" ];then
                                chip_id=`cat $pci_dev_path$bdf"/chip_id"`
                                if [ "$chip_id" != "0" ];then
                                        bdfs=$bdfs$bdf" "
                                fi
                        fi
                fi
    done

    echo "$bdfs"

    return 0;
}

get_bridge_bus_bdfs()
{
    local bdf bdfs bus_bdfs;
    bdfs="$1"
    bus_bdfs=""
        for bdf in $bdfs
        do
        if [ -e $pci_dev_path$bdf"/bus_name" ];then
            bus_name=`cat $pci_dev_path$bdf"/bus_name"`
            bus_bdfs=$bus_bdfs$bus_name" "
        fi
    done
    echo "$bus_bdfs"
}

remove_devs()
{
    local bdfs=$1
    local bdf
    for bdf in $bdfs
        do
                log "remove dev: "$bdf
        echo 1 > $pci_dev_path$bdf"/remove"
        bdf="${bdf/.2/.1}"
                log "remove dev: "$bdf
                echo 1 > $pci_dev_path$bdf"/remove"
        bdf=${bdf/.1/.0}
                log "remove dev: "$bdf
        echo 1 > $pci_dev_path$bdf"/remove"
        done
}

master_do_hotreset()
{
    local bdf bus_bdfs
    bus_bdfs="$1"

    for bdf in $bus_bdfs
        do
                log "set hot reset bus(43): "$bdf
        setpci -s "$bdf" 3e.b=43
        done

    sleep 1

    for bdf in $bus_bdfs
        do
                log "set hot reset bus(03): "$bdf
                setpci -s "$bdf" 3e.b=03
        done
}
rescan_bus()
{
        local bus_bdfs=$1
        local bdf
        for bdf in $bus_bdfs
        do
                log "rescan bus: "$bdf
                echo 1 > $pci_dev_path$bdf"/rescan"
        done
}

get_devs_num()
{
    local bdf bdfs
    bdfs=$1
    i=0
        for bdf in $bdfs
        do
        ((i++))
        done
    echo $i
}

hot_reset()
{
    if [ -e "$hotreset_status_file" ];then
        hotreset_status=`cat "$hotreset_status_file"`
        hotreset_status=${hotreset_status%.*}
    else
        hotreset_status="unknown"
    fi
     log "[INFO]hotreset_status:"$hotreset_status
    if [ "$hotreset_status" = "scan_success" ];then
        log "hot reset has already executed"
        return
    fi

    log "[INFO]try hot reset device"
    echo "start" > "$hotreset_status_file"

    check_euler_os_process
    if [ $? -ne 0 ];then
        log "[INFO]not euleros,not support hotreset"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    thread_num=`ps -ef | grep ascend_monitor | grep -v "grep"|wc -l`
    if [ $thread_num -gt 0 ]; then
        log "[INFO]ascend_monitor process exist"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    check_kernel_version
    check_user_process
        res=`echo $?`   # get return result
        if [ $res = "1" ]; then
                log "[INFO]user process exist, no hot reset"
                echo "abort" > "$hotreset_status_file"
                return
        fi

    local allDevs=`get_bdfs "all"`
    log "[INFO]allDevs: $allDevs"
    local masterDevs=`get_bdfs "master"`
    log "[INFO]masterDevs: $masterDevs"
    local slaveDevs=`get_bdfs "slave"`
    log "[INFO]slaveDevs: $slaveDevs"

    local devnum=`get_devs_num "$allDevs"`
    if [ $devnum != 8 ]; then
        log "[INFO]total dev num: "$devnum" not surport hotreset"
        echo "abort" > "$hotreset_status_file"
        return
    fi
    log "[INFO]devnum: $devnum"

    local masterDevsBus=`get_bridge_bus_bdfs "$masterDevs"`
    log "[INFO]masterDevsBus: $masterDevsBus"
    local slaveDevsBus=`get_bridge_bus_bdfs "$slaveDevs"`
    log "[INFO]slaveDevsBus: $slaveDevsBus"

    local busnum=`get_devs_num "$masterDevsBus"`
    if [ $busnum == 0 ]; then
        log "[INFO]master bus num: "$busnum" not surport hotreset"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    echo "device hot reset start"

    log "[INFO]1.set hotreset flag for bios"
    master_set_hotreset_flag "$masterDevs"

    log "[INFO]2.remove old version modules ko"
    remove_all_ko
    res=`echo $?`   # get return result
    if [ $res = "1" ]; then
        log "ko can not be removed"
        echo "abort" > "$hotreset_status_file"
        return
    fi

    log "[INFO]3.remove slave devices avoid host reset"
    remove_devs "$slaveDevs"
    sleep 1

    log "[INFO]4.set bus bridge to do hot reset"
    master_do_hotreset "$masterDevsBus"
    sleep 1

    log "[INFO]5.remove master device to redistribution bar address"
    remove_devs "$masterDevs"

    log "[INFO]6.rescan and wait all removed devices"
    local scan_devnum try_cnt
    scan_devnum=0
    try_cnt=0
    while [ $devnum != $scan_devnum ]
    do
        if (($try_cnt > 10)); then
            log "[WARNING]hot reset failed, all device num:"$devnum", rescan device num:"$scan_devnum
            echo "fail" > "$hotreset_status_file"
            return
        fi
        ((try_cnt++))

        sleep 1
        rescan_bus "$masterDevsBus"
        rescan_bus "$slaveDevsBus"
        allDevs=`get_bdfs "all"`
        scan_devnum=`get_devs_num "$allDevs"`

    done

    echo "device hot reset finish"
    log "[INFO]device hot reset finish"
    echo "scan_success."$(date +%s) > "$hotreset_status_file"
}

hot_reset

