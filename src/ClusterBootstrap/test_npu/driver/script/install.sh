#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
installInfo="/etc/ascend_install.info"
driverInStatusFile="/dev/shm/driver_installing_status_tmp"
username=HwHiAiUser
usergroup=HwHiAiUser
hotreset_status_file="/var/log/hotreset_status.log"
driverCrlStatusFile="/root/ascend_check/driver_crl_status_tmp"

OPERATION_LOGDIR="${ASCEND_SECLOG}"
OPERATION_LOGPATH="${OPERATION_LOGDIR}/operation.log"
LOG_OPERATION_INSTALL="Install"
LOG_OPERATION_UPGRADE="Upgrade"
LOG_OPERATION_UNINSTALL="Uninstall"
LOG_LEVEL_SUGGESTION="SUGGESTION"
LOG_LEVEL_MINOR="MINOR"
LOG_LEVEL_MAJOR="MAJOR"
LOG_LEVEL_UNKNOWN="UNKNOWN"
LOG_RESULT_SUCCESS="success"
LOG_RESULT_FAILED="failed"



log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}

errorUsage() {
    if [ $# -eq 1 ]; then
        echo "ERR_NO:0x0004;ERR_DES: Unrecognized parameters: ${1}. Try './xxx.run --help' for more information."
        exitLog 1
    else
        echo "ERR_NO:0x0004;ERR_DES: Unrecognized parameters. Try './xxx.run --help' for more information."
        rm -f $driverInStatusFile
        exit 1
    fi
}
# 日志文件轮询、备份
rotateLog() {
    echo "${logFile} {
        su root root
        daily
        size=5M
        rotate 3
        missingok
        create 440 root root
    }" > /etc/logrotate.d/ascend-install
}

# 开始安装前打印开始信息
startLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "InstallStart: $cur_date"
    echo "InstallStart: $cur_date" >> $logFile
}

exitLog() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "InstallEnd: ${cur_date}"
    echo "InstallEnd: ${cur_date}" >> $logFile
    rm -f $driverInStatusFile
    exit $1
}

installationCompletionMessage() {
    if [ -e "$hotreset_status_file" ]; then
        hotreset_status=`cat "$hotreset_status_file"`
        rm -f "$hotreset_status_file"
    else
        hotreset_status="unknown"
    fi
    if [ "$hotreset_status" = "success" ] || [ $docker = y ]; then
        echo -e "\033[32m Driver package install success! The new version takes effect immediately. \033[0m"
    else
        echo -e "\033[32m Driver package install success! \033[0m\033[31mReboot needed for installation/upgrade to take effect! \033[0m"
    fi
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
# 判断是否热复位失败
isHotresetFailed(){
    if [ "${hotreset_status}" = "scan_success"* ] || [ "${hotreset_status}" = "success" ]; then
        echo "false"
    else
        echo "true"
    fi
}

absolutePath(){
    flag1=$(echo $Install_Path_Param |grep "\./")''$(echo $Install_Path_Param |grep "~/")''$(echo $Install_Path_Param |grep "/\.")
    flag2=$(echo $Install_Path_Param |grep "^/")
    if [ ! -z $flag1  ] || [  -z $flag2  ]; then
        echo "ERR_NO:0x0003;ERR_DES:install-path only support absolute path , install failed"
        log "[ERROR]ERR_NO:0x0003;ERR_DES:install-path only support absolute path , install failed"
        exitLog 1
    fi
}
# 判断输入的指定路径是否存在
isValidPath() {
    [ $uninstall = y ] && return 0
    makeDefaultPath=n
    if [ "$input_install_path" = "" ]; then
        if [ -f $installInfo ]; then
            . $installInfo
            if [ ! -z $Driver_Install_Path_Param ]; then
                Install_Path_Param=$Driver_Install_Path_Param
            fi
        fi
    else
        Install_Path_Param=${input_install_path}
    fi

    absolutePath
    if [ ! -d $Install_Path_Param ]; then
        if [ $Install_Path_Param = "/usr/local/Ascend" ]; then
            mkdir -p $Install_Path_Param
            chown $username:$usergroup $Install_Path_Param > /dev/null 2>&1
            chmod 550 $Install_Path_Param > /dev/null 2>&1
            makeDefaultPath=y
        else
            echo "ERR_NO:0x0003;ERR_DES:install path ${Install_Path_Param} not exists, driver package install failed"
            log "[ERROR]ERR_NO:0x0003;ERR_DES:install ${Install_Path_Param} path not exists, driver package install failed"
            exitLog 1
        fi
    fi

    su - $username -c "cd $Install_Path_Param >> /dev/null 2>&1"
    if [ ! $? = 0 ]; then
        if [ $makeDefaultPath = y ]; then
            rm -rf $Install_Path_Param
        fi
        echo "ERR_NO:0x0003;ERR_DES:The $username do not have the permission to access $Install_Path_Param, please reset the directory to a right permission."
        log "[ERROR]ERR_NO:0x0003;ERR_DES:The $username do not have the permission to access $Install_Path_Param, please reset the directory to a right permission."
        exitLog 1
    fi
}

# 检查是否存在进程
checkProcess(){
    name=`ps -ef | awk '$2=='$$'{print $10}'|rev|cut -d "/" -f1|rev`
    shellname=`echo $0 |rev |cut -d "/" -f1 |rev`
    process=`ps -ef | grep -v "grep" | grep -w "$shellname" |grep -w "$name"`
    pid=`echo "$process" | awk -F ' ' '{print $2}'`
    ret=`echo "$process" | awk -F ' ' '{print $3}' | grep -v "$pid" | wc -l`
    if [ $ret -gt 1 ]; then
        log "[ERROR]ERR_NO:0x0094;ERR_DES:There is already a process being executed,please do not execute multiple tasks at the same time"
        log "[INFO]$name;$shellname;$ret;$process"
        echo "ERR_NO:0x0094;ERR_DES:There is already a process being executed,please do not execute multiple tasks at the same time"
        exitLog 1
    fi
}

# 检查用户是否存在
checkUser(){
    ret=`cat /etc/passwd | cut -f1 -d':' | grep -w "$1" -c`
    if [ $ret -le 0 ]; then
        return 1
    else
        return 0
    fi
}

# 校验user和group的关联关系
checkGroup(){
    group_user_related=`groups "$2"|awk -F":" '{print $2}'|grep -w "$1"`
    if [ "${group_user_related}x" != "x" ];then
        return 0
    else
        return 1
    fi
}

# 检查当前系统是否满足安装条件
checkInstallOS(){
    . ./driver/script/common_func.inc
    get_system_info
    # 在scene.info中匹配os信息,不区分大小写
    for item in $os
    do
        if echo $item | grep -i "^${HostOsName}$">/dev/null;then
            log "[INFO]os($HostOsName) match success"
            return
        fi
    done
    log "[WARNING]os($HostOsName) not support"
    if [ $quiet = n ]; then
        echo "[WARNING]os($HostOsName) not match scene.info($item),do you want to continue installing?  [y/n] "
        while true
        do
            read yn
            if [ "$yn" = n ]; then
                echo "stop installation!"
                exitLog 0;
            elif [ "$yn" = y ]; then
                break;
            else
                echo "ERR_NO:0x0002;ERR_DES:input error, please input again!"
        fi
        done
    fi
}

# 检查是否满足安装条件
checkInstallCondition() {
    [ $uninstall = y ] && return 0
    checkUser "${username}"
    if [ $? -ne 0 ];then
        log "[ERROR]ERR_NO:0x0091;ERR_DES:${username} not exists! Please add ${username}"
        echo "ERR_NO:0x0091;ERR_DES:${username} not exists! Please add ${username}"
        exitLog 1
    fi

    checkGroup "${usergroup}" "${username}"
    if [ $? -ne 0 ];then
        log "[ERROR]ERR_NO:0x0096;ERR_DES:${usergroup} not right! Please check the relatianship of ${username} and ${usergroup}"
        echo "ERR_NO:0x0096;ERR_DES:${usergroup} not right! Please check the relatianship of ${username} and ${usergroup}"
        exitLog 1
    fi
    echo "set username and usergroup, ${username}:${usergroup}"
    checkInstallOS

    if [ ! "$arch" = "$HostArch" ];then
        if [ ! "$HostArch" = "unknown" ];then
            log "[ERROR]ERR_NO:0x0092;ERR_DES:arch($HostArch) not support"
            echo "ERR_NO:0x0092;ERR_DES:arch($HostArch) not support"
            exitLog 1
        fi
        log "[WARNING]ERR_NO:0x0092;ERR_DES:arch($HostArch) may not support"
        echo "[WARNING]arch($HostArch) may not support"
    fi

}

getUserInfo() {
    if [ -f ${installInfo} ]; then
        . $installInfo
        if [ ${input_install_username} = y ] || [ ${input_install_usergroup} = y ]; then
            if [ "x${UserName}" = "x" ]; then
                sub_file=`echo $Install_Path_Param |awk -F"/" '{print $NF}'`
                username_cur=`ls -l "${Install_Path_Param}/../" |grep -w "${sub_file}" |awk '{print $3}'`
                if [ "x${username}" != "x${username_cur}" ]; then
                    log "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    echo "ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    exitLog 1
                fi
            else
                if [ "x${username}" != "x${UserName}" ]; then
                    log "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    echo "ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    exitLog 1
                else
                    username=${UserName}
                fi
            fi

            if [ "x${UserGroup}" = "x" ]; then
                sub_file=`echo $Install_Path_Param |awk -F"/" '{print $NF}'`
                usergroup_cur=`ls -l "${Install_Path_Param}/../" |grep -w "${sub_file}" |awk '{print $4}'`
                if [ "x${usergroup}" != "x${usergroup_cur}" ]; then
                    log "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    echo "ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    exitLog 1
                fi
            else
                if [ "x${usergroup}" != "x${UserGroup}" ]; then
                    log "[ERROR]ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    echo "ERR_NO:0x0095;ERR_DES:The user and group are not same with last installation,do not support overwriting installation"
                    exitLog 1
                else
                    usergroup=${UserGroup}
                fi
            fi
        fi
        if [ ${input_install_username} = n ] || [ ${input_install_usergroup} = n ]; then
            if [ x${UserName} != "x" ]; then
                if [ ${UserName} = "root" ]; then
                    echo "ERR_NO:0x0091;ERR_DES:username not permission for root, check /etc/ascend_install.info"
                    log "[ERROR]ERR_NO:0x0091;ERR_DES:username not permission for root, check /etc/ascend_install.info"
                    exitLog 1
                else
                    username=${UserName}
                fi
            fi
            if [ x${UserGroup} != "x" ]; then
                if [ ${UserGroup} = "root" ]; then
                    echo "ERR_NO:0x0091;ERR_DES:usergroup not permission for root, check /etc/ascend_install.info"
                    log "[ERROR]ERR_NO:0x0091;ERR_DES:usergroup not permission for root, check /etc/ascend_install.info"
                    exitLog 1
                else
                    usergroup=${UserGroup}
                fi
            fi
        fi
    fi
}
# 检查root权限
isRoot() {
    if [ $(id -u) -ne 0 ]; then
        echo "ERR_NO:0x0093;ERR_DES:do not have root permission, install failed,\
         please use root permission!"
        rm -f $driverInStatusFile
        exit 1
    fi
}

# 记录后台run包相关进程
getAllOfDaemons(){
    back_proc="/driver/tools/ada /driver/tools/slogd \
                /driver/tools/sklogd /driver/tools/hdcd"
    ps -ef > hiai_get_tmp_daemon.log
    for item in $back_proc
    do
        cat hiai_get_tmp_daemon.log | grep $item >/dev/null 2>&1
        if [ $? -eq 0 ];then
            log "[INFO]${item##*/} is up"
        else
            log "[INFO]${item##*/} is down"
        fi
    done
    rm hiai_get_tmp_daemon.log
}


# 获取安装目录下的完整版本号  version2
getVersionInstalled() {
    version2="none"
    if [ -f $1/driver/version.info ]; then
        . $1/driver/version.info
        version2=${Version}
    fi
    echo $version2
}
# 获取run包中的完整版本号  version1
getVersionInRunFile() {
    version1="none"
    if [ -f ./version.info ]; then
        . ./version.info
        version1=${Version}
    fi
    echo $version1
}

logBaseVersion() {
    if [ -f ${installInfo} ];then
        . $installInfo
        if [ ! -z $Driver_Install_Path_Param ]; then
            installed_version=$(getVersionInstalled $Driver_Install_Path_Param)
            if [ ! "${installed_version}"x = ""x ]; then
                echo "INFO: base version is ${installed_version}."
                log "[INFO] base version is ${installed_version}."
                return 0
            fi
        fi
    fi
    log "[INFO] base version was destroyed or not exist."
}

# 判断命令是否返回失败，失败则报错返回
error() {
    # 目前正在执行的操作类型
    local operation="$1"
    # 上一个命令的返回码
    local retcode="$2"
    # （可选）打印的消息
    local msg="$3"
    if [ ${retcode} -ne 0 ]; then
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
    fi

    if [ $# -eq 3 ]; then
        if [ ${retcode} -ne 0 ]; then
            echo "${msg}"
            log "[ERROR]${msg}"
            exitLog 1
        fi
    elif [ $# -eq 2 ]; then
        if [ ${retcode} -ne 0 ]; then
            exitLog 1
        fi
    fi
}

#安装或升级前，更新/etc/ascend_install.info
updateInstallInfo() {
    if [ -f $installInfo ]; then
        . $installInfo
        if [ -z $UserName ]; then
            echo "UserName=$username" >> $installInfo
        fi

        if [ -z $UserGroup ]; then
            echo "UserGroup=$usergroup" >> $installInfo
        fi

        if [ -z $Driver_Install_Type ]; then
            echo "Driver_Install_Type=$installType" >>$installInfo
        else
            sed -i "/Driver_Install_Type=/c Driver_Install_Type=$installType" $installInfo
        fi

        if [ -z $Driver_Install_Path_Param ]; then
            echo "Driver_Install_Path_Param=$Install_Path_Param" >>$installInfo
        else
            sed -i "/Driver_Install_Path_Param=/c Driver_Install_Path_Param=$Install_Path_Param" $installInfo
        fi
    else
        echo "UserName=$username" > $installInfo
        echo "UserGroup=$usergroup" >> $installInfo
        echo "Driver_Install_Type=$installType" >>$installInfo
        echo "Driver_Install_Path_Param=$Install_Path_Param" >>$installInfo
    fi
    chmod 644 $installInfo

}

makeDefaultDir() {
    if [ ! -d $Install_Path_Param ] && [ $Install_Path_Param = "/usr/local/Ascend" ]; then
        mkdir -p $Install_Path_Param
        chown $username:$usergroup $Install_Path_Param > /dev/null 2>&1
        chmod 550 $Install_Path_Param > /dev/null 2>&1
    fi
}

installRun() {
    makeDefaultDir
    updateInstallInfo
    oldVersion=$(getVersionInstalled $Install_Path_Param)
    ./driver/script/run_driver_install.sh $Install_Path_Param $installType
    if [ $? -eq 0 ];then
        installationCompletionMessage
    else
        echo -e $1" driver failed, please retry after uninstall and reboot!"
    fi

}

uninstallRun() {
    operation="${LOG_OPERATION_UNINSTALL}"
    . $installInfo
    if [ $uninstall = n ] && [ ! $Driver_Install_Type = "docker" ]; then
        . ./driver/script/device_crl_check.sh
        #check CRL of images
        device_images_crl_check
        if [ $? -ne 0 ];then
            echo -e "\033[31m device_images_crl_check, details in : $logFile \033[0m"
            log "[ERROR]new device crl check fail, stop upgrade"
            rm -f $driverCrlStatusFile
            exitLog 1
        fi
    fi
    if [ -f $Driver_Install_Path_Param/host_sys_stop.sh ]; then
        $Driver_Install_Path_Param/host_sys_stop.sh
    else
        ./host_sys_stop.sh
    fi
    if [ -f $Driver_Install_Path_Param/host_servers_remove.sh ]; then
        $Driver_Install_Path_Param/host_servers_remove.sh
    else
        ./host_servers_remove.sh
    fi
    uninstall_result=n
    if [ -f $Driver_Install_Path_Param/driver/script/run_driver_uninstall.sh ]; then
        $Driver_Install_Path_Param/driver/script/run_driver_uninstall.sh --uninstall $Driver_Install_Path_Param >/dev/null 2>&1
        if [ $? -eq 0 ];then
            uninstall_result=y
        fi
    fi
    if [ $uninstall_result = n ]; then
        ./driver/script/run_driver_uninstall.sh --uninstall $Driver_Install_Path_Param >/dev/null 2>&1
        if [ $? -eq 0 ]; then
            uninstall_result=y
        else
            log "[WARNING]uninstall driver failed"
        fi
    fi

    if [ $uninstall = y ] && [ $uninstall_result = n ]; then
        echo "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed;Detail message in ${ASCEND_SECLOG}/ascend_install.log"
        logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_FAILED}" "${installType}" "${all_parma}"
        log "[ERROR]ERR_NO:0x0090;ERR_DES:uninstall driver failed"
        exitLog 1
    fi

    if [ $uninstall_result = y ]; then
        log "[INFO]uninstall driver success"
        if [ $uninstall = y ]; then
            sed -i '/Driver_Install_Path_Param=/d' $installInfo
            sed -i '/Driver_Install_Type=/d' $installInfo
            if [ ` grep -c -i "Install_Path_Param" $installInfo ` -eq 0 ]; then
                rm -f $installInfo
            fi
            logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_SUCCESS}" "${installType}" "${all_parma}"
            uninstallationCompletionMessage
            exitLog 0
        fi
    fi
    unset Driver_Install_Path_Param

}

changeLogMode() {
    if [ ! -f $logFile ]; then
        touch $logFile
        chmod 640 $logFile
    fi
    chmod 640 $logFile
}

# 创建文件夹
createFolder() {
    if [ ! -d /var/log ]; then
        mkdir -p /var/log
        chmod 755 /var/log
    fi
}

logOperation() {
    local operation="$1"
    local start_time="$2"
    local runfilename="$3"
    local result="$4"
    local install_type="$5"
    local cmdlist="$6"
    local level

    if [ "${operation}" = "${LOG_OPERATION_INSTALL}" ]; then
        level="${LOG_LEVEL_SUGGESTION}"
    elif [ "${operation}" = "${LOG_OPERATION_UPGRADE}" ]; then
        level="${LOG_LEVEL_MINOR}"
    elif [ "${operation}" = "${LOG_OPERATION_UNINSTALL}" ]; then
        level="${LOG_LEVEL_MAJOR}"
    else
        level="${LOG_LEVEL_UNKNOWN}"
    fi

    if [ ! -d "${OPERATION_LOGDIR}" ]; then
        mkdir -p ${OPERATION_LOGDIR}
        chmod 750 ${OPERATION_LOGDIR}
    fi

    if [ ! -f "${OPERATION_LOGPATH}" ]; then
        touch ${OPERATION_LOGPATH}
        chmod 640 ${OPERATION_LOGPATH}
    fi

    if [ $upgrade = y ] || [ $uninstall = y ]; then
        echo "${operation} ${level} root ${start_time} 127.0.0.1 ${runfilename} ${result}"\
            "cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}
    else
        echo "${operation} ${level} root ${start_time} 127.0.0.1 ${runfilename} ${result}"\
            "install_type=${install_type}; cmdlist=${cmdlist}." >> ${OPERATION_LOGPATH}
    fi
}

uniqueMode() {
    if [ ! -z $installType ]; then
        echo "[ERROR]ERR_NO:0x0004;ERR_DES:only support one type: full/run/docker/upgrade, operation failed!"
        log "[ERROR]ERR_NO:0x0004;ERR_DES:only support one type: full/run/docker/upgrade, operation failed!"
        exitLog 1
    fi
}

unsupportParam() {
    # version uninstall只允许单独使用
    if [ $version = y ] || [ $uninstall = y ]; then
        if [ $i -gt 1 ]; then
            log "[ERROR]ERR_NO:0x0004;ERR_DES:version/uninstall can't used with other parameters."
            echo "ERR_NO:0x0004;ERR_DES:version/uninstall can't used with other parameters."
            exitLog 1
        fi
    fi
    if [ $quiet = y ] && [ $full_install = n ] && [ $docker = n ] && [ $run = n ] && [ $upgrade = n ]; then
        log "[ERROR]ERR_NO:0x0004;ERR_DES:quiet param need used with full/run/docker/upgrade."
        echo "ERR_NO:0x0004;ERR_DES:quiet param need used with full/run/docker/upgrade."
        exitLog 1
    fi
    if [ $input_path_flag = y ] && [ $full_install = n ] && [ $docker = n ] && [ $run = n ]; then
        log "[ERROR]ERR_NO:0x0004;ERR_DES:install-path param need used with full/run/docker."
        echo "ERR_NO:0x0004;ERR_DES:install-path param need used with full/run/docker."
        exitLog 1
    fi

    #升级不支持指定路径
    if [ $upgrade = y ] && [ $input_path_flag = y ]; then
        log "[ERROR]ERR_NO:0x0004;ERR_DES:Unsupported install-path in upgrade, operation failed."
        echo "ERR_NO:0x0004;ERR_DES:Unsupported install-path in upgrade, operation failed."
        exitLog 1
    fi

    # 指定用户以及属组仅支持安装场景,不支持卸载、更新的场景
    if [ "${input_install_username}" = y ] || [ "${input_install_usergroup}" = y ]; then
        if [ "${input_install_username}" = y ] && [ "${input_install_usergroup}" = y ]; then
            if [ $full_install = n ] && [ $docker = n ] && [ $run = n ]; then
                log "[ERROR]ERR_NO:0x0004;ERR_DES:install-username and install-usergroup params need used with full/run/docker."
                echo "ERR_NO:0x0004;ERR_DES:install-username and install-usergroup params need used with full/run/docker."
                exitLog 1
            fi
            if [ "$upgrade" = y ] || [ "$uninstall" = "y" ]; then
                echo "ERR_NO:0x0004;ERR_DES:Unsupported parameters, only the installation scenario specifies is supported for the specified username or usergroup."
                echo "Please input the right params and try again."
                log "[ERROR]ERR_NO:0x0004;ERR_DES:Unsupported parameters, only the installation scenario specifies is supported for the specified username or usergroup."
                exitLog 1
            fi
        else
            # 目前方案为需同时输入指定的用户和用户属组，否则报错
            echo "ERR_NO:0x0004;ERR_DES:Input username and usergroup, only the installation scenario specifies is supported for the specified username or usergroup."
            echo "Please input the right params and try again."
            log "[ERROR]ERR_NO:0x0004;ERR_DES:Input username and usergroup, only the installation scenario specifies is supported for the specified username or usergroup."
            exitLog 1
        fi
    fi
}

crossVersion() {
    if [ -f /etc/HiAI_install.info ]; then
        . /etc/HiAI_install.info
        if [ -z $Install_Path_Param ]; then
            echo "ERR_NO:0x0094;ERR_DES:Operation failed, An all-in-one RUN package is found, which needs to be uninstalled before proceeding."
        else
            echo "ERR_NO:0x0094;ERR_DES:Operation failed, An all-in-one RUN package is found in $Install_Path_Param, which needs to be uninstalled before proceeding."
        fi
        exit
    fi
}
crossVersion

if [ -f $driverInStatusFile ]; then
    echo "ERR_NO:0x0094;ERR_DES:There is already a process being executed,please do not execute multiple tasks at the same time"
    exit 1
fi

touch $driverInStatusFile
chmod 440 $driverInStatusFile
trap "rm -f $driverInStatusFile;exit 1" INT QUIT KILL TERM TSTP

if [ ! -d "${ASCEND_SECLOG}" ]; then
    mkdir -p ${ASCEND_SECLOG}
    chmod 750 ${ASCEND_SECLOG}
fi

if [ -f $installInfo ]; then
    chmod 644 $installInfo
    . $installInfo
    if [ ! -z $UserName ]; then
        username=$UserName
    fi
    if [ ! -z $UserGroup ]; then
        usergroup=$UserGroup
    fi
fi

runfilename=$(expr substr $1 5 $(expr ${#1} - 4))
full_install=n
uninstall=n
upgrade=n
input_install_path=""
input_path_flag=n
input_install_username=n
input_install_usergroup=n
Install_Path_Param="/usr/local/Ascend"
operation="Install"
quiet=n
run=n
version=n
installType=""
docker=n
# 过滤run包自带参数以及拦截执行目录带有空格的情形,此逻辑禁止删除
i=0
while true
do
    if [ x"$1" = x"" ];then
        break
    fi
    if [ "`expr substr $1 1 2 `" = "--" ]; then
       i=`expr $i + 1`
    fi
    if [ $i -gt 2 ]; then
        break
    fi
    shift 1
done
start_time=$(date +"%Y-%m-%d %H:%M:%S")
all_parma="$@"
if [ -z "$all_parma" ]; then
    errorUsage
fi

startLog
echo "InstallLogFile: ${ASCEND_SECLOG}/ascend_install.log"
log "[INFO]UserCommand: $runfilename $all_parma"

. ./driver/scene.info
isRoot
createFolder
changeLogMode
checkProcess
logBaseVersion

i=0
while true
do
    case "$1" in
    --uninstall)
    i=$(( $i + 1 ))
    operation=${LOG_OPERATION_UNINSTALL}
    uninstall=y
    shift
    ;;
    --upgrade)
    i=$(( $i + 1 ))
    operation=${LOG_OPERATION_UPGRADE}
    upgrade=y
    shift
    ;;
    --full)
    i=$(( $i + 1 ))
    operation=${LOG_OPERATION_INSTALL}
    uniqueMode
    installType="full"
    full_install=y
    shift
    ;;
    --docker)
    i=$(( $i + 1 ))
    docker=y
    uniqueMode
    installType="docker"
    shift
    ;;
    --version)
    i=$(( $i + 1 ))
    version=y
    shift
    ;;
    --quiet)
    i=$(( $i + 1 ))
    quiet=y
    shift
    ;;
    --run)
    i=$(( $i + 1 ))
    uniqueMode
    installType="run"
    run=y
    shift
    ;;
    --install-username=*)
    i=$(( $i + 1 ))
    input_install_username=y
    username=`echo $1 | cut -d"=" -f2 `
    if [ -z $username ]; then
        echo "ERR_NO:0x0091;ERR_DES:input_install_username is empty"
        log "[ERROR]ERR_NO:0x0091;ERR_DES:input_install_username is empty"
        exitLog 1
    elif [ $username = "root" ]; then
        echo "ERR_NO:0x0091;ERR_DES:username not permission for root"
        log "[ERROR]ERR_NO:0x0091;ERR_DES:username not permission for root"
        exitLog 1
    else
        shift
    fi
    ;;
    --install-usergroup=*)
    i=$(( $i + 1 ))
    input_install_usergroup=y
    usergroup=`echo $1 | cut -d"=" -f2 `
    if [ -z $usergroup ]; then
        echo "ERR_NO:0x0091;ERR_DES:input_install_group is empty"
        log "[ERROR]ERR_NO:0x0091;ERR_DES:input_install_group is empty"
        exitLog 1
    elif [ $usergroup = "root" ]; then
        echo "ERR_NO:0x0091;ERR_DES:usergroup not permission for root"
        log "[ERROR]ERR_NO:0x0091;ERR_DES:usergroup not permission for root"
        exitLog 1
    else
        shift
    fi
    ;;
    --install-path=*)
    i=$(( $i + 1 ))
    input_path_flag=y
    temp_path=`echo $1 | cut -d"=" -f2 `
        # 去除指定安装目录后所有的 "/"
    input_install_path=`echo $temp_path | sed "s/\/*$//g"`
    if [ $input_install_path"x" = "x" ]; then
        echo "ERR_NO:0x0003;ERR_DES:install path is null, install failed"
        log "[ERROR]ERR_NO:0x0003;ERR_DES:install path is null, install failed"
        exitLog 1
    else
        shift
    fi
    ;;
    -*)
    errorUsage $1
    ;;
    *)
    break
    ;;
    esac
done

unsupportParam

if [ $version = y ]; then
    echo "driver version :"$(getVersionInRunFile)
    rm -f $driverInStatusFile
    exit 0
fi

trap "logOperation \"${operation}\" \"${start_time}\" \"${runfilename}\" \"${LOG_RESULT_FAILED}\" \"${installType}\" \"${all_parma}\";rm -f $driverInStatusFile;exit 1" \
INT QUIT KILL TERM TSTP
if [ $upgrade = y ]; then
    uniqueMode
    if [ -f ${installInfo} ];then
        . $installInfo
        if [ -z $Driver_Install_Type ]; then
            installType="full"
        else
            installType=$Driver_Install_Type
        fi
    else
        installType="full"
    fi
fi

getUserInfo
getAllOfDaemons
rotateLog
checkInstallCondition
isValidPath

# 环境上是否已安装过run包
if [ -f ${installInfo} ]; then
    . $installInfo
    #首次安装场景
    if [ -z $Driver_Install_Path_Param ]; then
        # 卸载场景
        if [ $uninstall = y ]; then
            error "${LOG_OPERATION_UNINSTALL}" 1 "ERR_NO:0x0080;ERR_DES:Driver package is not installed on this device, uninstall failed"
        # 安装场景
        elif [ $full_install = y ] || [ $upgrade = y ] || [ $run = y ] || [ $docker = y ]; then
            installRun "install"
            operation="${LOG_OPERATION_INSTALL}"
        fi
    else
        version1=$(getVersionInRunFile)
        version2=$(getVersionInstalled $Driver_Install_Path_Param)
        # 卸载场景
        if [ $uninstall = y ]; then
            uninstallRun
        # 覆盖安装场景
        elif [ $full_install = y ] || [ $run = y ] || [ $docker = y ]; then
            if [ ! $version2"x" = "x" ] && [ ! "$version2" = "none" ] && [ $quiet = n ]; then
                # 判断是否要覆盖式安装
                echo "Driver package has been installed on this device, the version is"\
                "${version2}, and the version of this package is ${version1},"\
                "do you want to continue installing?  [y/n] "
                while true
                do
                    read yn
                    if [ "$yn" = n ]; then
                        echo "stop installation!"
                        exitLog 0;
                    elif [ "$yn" = y ]; then
                        break;
                    else
                        echo "ERR_NO:0x0002;ERR_DES:input error, please input again!"
                    fi
                done
            fi
            uninstallRun
            installRun "install"
            operation="${LOG_OPERATION_INSTALL}"
        # 升级场景
        elif [ $upgrade = y ]; then
            uninstallRun
            installRun "upgrade"
            operation="${LOG_OPERATION_UPGRADE}"
        fi
    fi
else
    # 卸载场景
    if [ $uninstall = y ]; then
        error "${LOG_OPERATION_UNINSTALL}" 1 "ERR_NO:0x0080;ERR_DES:Driver package is not installed on this device, uninstall failed"
    # 安装场景
    elif [ $full_install = y ] || [ $upgrade = y ] || [ $run = y ] || [ $docker = y ]; then
        installRun "install"
        operation="${LOG_OPERATION_INSTALL}"
    fi
fi

logOperation "${operation}" "${start_time}" "${runfilename}" "${LOG_RESULT_SUCCESS}" "${installType}" "${all_parma}"
if [ $docker = y ]; then
    exitLog 0
else
    if [ "$(isHotresetFailed)" = "true" ]; then
        exitLog 2
    else
        exitLog 0
    fi
fi
