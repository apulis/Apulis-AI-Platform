#!/bin/sh
installInfo="/etc/ascend_install.info"
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
driverInStatusFile="/dev/shm/driver_installing_status_tmp"
hotreset_status_file="/var/log/hotreset_status.log"

LOG_LEVEL_MAJOR="MAJOR"
OPERATION_LOGPATH="${ASCEND_SECLOG}/operation.log"
LOG_OPERATION_UNINSTALL="Uninstall"
LOG_RESULT_SUCCESS="success"
LOG_RESULT_FAILED="failed"
log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
    return 0
}

error() {
    local msg="$1"
    echo "${msg}"
    log "[ERROR]${msg}"
    logOperation "${start_time}" "${LOG_RESULT_FAILED}"
    exitLog 1
}

startLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "InstallStart: $cur_date" >> $logFile
}

exitLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "InstallEnd: ${cur_date}" >> $logFile
    rm -f $driverInStatusFile
    exit $1
}

# 检查root权限
isRoot() {
    if [ $(id -u) -ne 0 ]; then
        echo "ERR_NO:0x0093;ERR_DES:do not have root permission, operation failed,\
         please use root permission!"
        rm -f $driverInStatusFile
        exit 1
    fi
}

# 创建文件夹
createFolder() {
    if [ ! -d /var/log ]; then
        mkdir -p /var/log
        chmod 755 /var/log
    fi

    if [ ! -d "${ASCEND_SECLOG}" ]; then
        mkdir -p ${ASCEND_SECLOG}
        chmod 750 ${ASCEND_SECLOG}
    fi
}

changeLogMode() {
    if [ ! -f $logFile ]; then
        touch $logFile
    fi
    chmod 640 $logFile
}

logBaseVersion() {
    if [ -f $uninstallPath/driver/version.info ];then
        . $uninstallPath/driver/version.info
        installed_version=${Version}
        if [ ! "${installed_version}"x = ""x ]; then
            echo "INFO: base version is ${installed_version}."
            log "[INFO] base version is ${installed_version}."
            return 0
        fi
    fi
    log "[INFO] base version was destroyed or not exist."
}

uninstallationCompletionMessage() {
        if [ -e "$hotreset_status_file" ]; then
            hotreset_status=`cat "$hotreset_status_file"`
            hotreset_status=${hotreset_status%%.*}
            rm -f "$hotreset_status_file"
        else
            hotreset_status="unknown"
        fi
        if [ "$hotreset_status" = "scan_success" ] || [ $Driver_Install_Type = "docker" ]; then
            echo -e "\033[32m Driver package uninstall success! Uninstallation takes effect immediately. \033[0m"
        else
            echo -e "\033[32m Driver package uninstall success! \033[0m\033[31mReboot needed for uninstallation to take effect! \033[0m"
        fi
}

logOperation() {
    local operation=${LOG_OPERATION_UNINSTALL}
    local start_time="$1"
    local runfilename="Ascend910-driver-"$installed_version
    local result="$2"
    local cmdlist="none"
    local level="${LOG_LEVEL_MAJOR}"

    if [ ! -f "${OPERATION_LOGPATH}" ]; then
        touch ${OPERATION_LOGPATH}
        chmod 640 ${OPERATION_LOGPATH}
    fi

    if [ $quiet = y ]; then
        cmdlist="--quiet"
    fi

    echo "${operation} ${level} root ${start_time} 127.0.0.1 ${runfilename} ${result}"\
    "cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}

}

uninstallRun(){
    uninstall_result=n
    if [ -f $uninstallPath/host_sys_stop.sh ]; then
        $uninstallPath/host_sys_stop.sh
    fi

    if [ -f $uninstallPath/host_servers_remove.sh ]; then
        cd $uninstallPath
        $uninstallPath/host_servers_remove.sh
        cd - > /dev/null 2>&1
    fi

    if [ -f $uninstallPath/driver/script/run_driver_uninstall.sh ]; then
        $uninstallPath/driver/script/run_driver_uninstall.sh --uninstall $uninstallPath > /dev/null 2>&1
        if [ $? -eq 0 ];then
            uninstall_result=y
        fi
    else
        error "ERR_NO:0x0080;ERR_DES:run_driver_uninstall.sh does not existed, uninstall driver failed"
    fi

    if [ $uninstall_result = n ]; then
        echo "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed;Detail message in ${ASCEND_SECLOG}/ascend_install.log"
        log "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed"
        logOperation "${start_time}" "${LOG_RESULT_FAILED}"
        exitLog 1
    fi

    if [ $uninstall_result = y ]; then
        log "[INFO]uninstall driver success"
        if [ -f $installInfo ]; then
            . $installInfo
            sed -i '/Driver_Install_Path_Param=/d' $installInfo
            sed -i '/Driver_Install_Type=/d' $installInfo
            if [ ` grep -c -i "Install_Path_Param" $installInfo ` -eq 0 ]; then
                rm -f $installInfo
            fi
        fi
        uninstallationCompletionMessage
        logOperation "${start_time}" "${LOG_RESULT_SUCCESS}"
        exitLog 0
    fi
}
if [ -f $driverInStatusFile ]; then
    echo "ERR_NO:0x0094;ERR_DES:There is already a process being executed,please do not execute multiple tasks at the same time"
    exit 1
fi

touch $driverInStatusFile
chmod 440 $driverInStatusFile
trap "rm -f $driverInStatusFile;exit 1" INT QUIT KILL TERM TSTP

start_time=$(date +"%Y-%m-%d %H:%M:%S")
shellPath=$(cd "$(dirname "$0")";pwd)
uninstallPath=$(cd $shellPath/../../;pwd)
installed_version="none"
quiet=n
isRoot
createFolder
changeLogMode
logBaseVersion
startLog

while true
do
    case "$1" in
    --quiet)
        quiet=y
        shift
        ;;
    *)
        if [ ! "x$1" = "x" ]; then
            error "ERR_NO:0x0004;ERR_DES: Unrecognized parameters: $1. Only support '--quiet'."
        fi
        break
        ;;
    esac
done
trap "logOperation \"${start_time}\" \"${LOG_RESULT_FAILED}\";rm -f $driverInStatusFile;exit 1" INT QUIT KILL TERM TSTP
uninstallRun

