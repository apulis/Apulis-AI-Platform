#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
logFile="${ASCEND_SECLOG}/ascend_install.log"
installInfo="/etc/ascend_install.info"
crldir="/root/ascend_check"
crltool="$crldir/ascend_910_check.bin"
driverCrlStatusFile="/root/ascend_check/driver_crl_status_tmp"
sourcedir=$PWD/driver
. $installInfo


log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}

#get current environment images crl when crl not exist in /root/ascend_check
gen_cur_image_crl_check_valid() {
    local ret=0
    log "[INFO]case one start: generate new images crl, check vaild and cms "

    if [ ! -e $crldir/ascend_910.crl ] ;then
        touch $crldir/ascend_910.crl
        chmod 400 $crldir/ascend_910.crl
        log "[INFO]touch ascend_910.crl success"
    fi
    if [ -e $crldir/ascend_910_check.bin ]; then
         crltool=$crldir/ascend_910_check.bin
    else
         crltool=$sourcedir/tools/ascend_910_check.bin
    fi
    $crltool 0 $sourcedir/device/tee.bin $crldir/ascend_910.crl
    if [ $? -ne 0 ]; then
        log "[INFO]1.generate new images crl fail, direct install "
        rm -rf $crldir/ascend_910.crl
        return 0
    fi
    log "[INFO]1.generate new images crl success , start check valid"
    #校验有效性，输入：2，文件名字 输出：0（正确），其他为非法
    $crltool 2  $crldir/ascend_910.crl
    if [ $? -ne 0 ]; then
        log "[ERROR]2.check new images crl failed, stop install"
        rm -rf $crldir/ascend_910.crl
        return 1
    fi
    log "[INFO]2.check new images crl success,start check cms"
    #验签，输入：3，较新的crl，要升级的images
    $crltool 3  $crldir/ascend_910.crl  $sourcedir/device/tee.bin
    if [ $? -ne 0 ]; then
        log "[ERROR]3.check new images crl cms failed, stop install"
        rm -rf $crldir/ascend_910.crl
        return 1
    fi
    log "[INFO]3.check new images crl cms success, update tool and crl"
    if [ -e $crldir/ascend_910_check.bin ]; then
        rm -f $crldir/ascend_910_check.bin
    fi
    cp -f $sourcedir/tools/ascend_910_check.bin $crldir/ascend_910_check.bin
    chmod 500 $crldir/ascend_910_check.bin
    cp -f $sourcedir/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
    chmod 500 $crldir/ascend_upgrade_crl.sh
    log "[INFO]4.update /root/ascend_check/ascend_910_check.bin, ascend_upgrade_crl.sh and crl success"
    log "[INFO]case one end: generate new images crl, check vaild and cms "
    return 0
    
}
#获取新的imeages的crl文件，并和/root/ascend_check/下的crl文件比较（该路径下crl必定存在）
gen_cur_image_crl_compare_newer() {
    local ret=0
    local new_tool_flag=0
    log "[INFO]case two start: generate new images crl, compare newer and check cms "

    if [ ! -e $crldir/ascend_910_check.bin ]; then
        cp -f $sourcedir/tools/ascend_910_check.bin $crldir/ascend_910_check.bin
        chmod 500 $crldir/ascend_910_check.bin
        cp -f $sourcedir/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
        chmod 500 $crldir/ascend_upgrade_crl.sh
        new_tool_flag=1
        log "[INFO]use new ascend_910_check.bin and ascend_upgrade_crl.sh in runpackage"
    fi
    #解压run包images的crl文件，cur_ascend_910.crl，输入：类型(0:获取文件，1:比较,2检查有效性，3验签)，images,路径+文件名，输出：返回值0成功，其他表示失败
    touch $crldir/cur_ascend_910.crl
    chmod 400 $crldir/cur_ascend_910.crl
    $crldir/ascend_910_check.bin 0 $sourcedir/device/tee.bin $crldir/cur_ascend_910.crl
    if [ $? -ne 0 ]; then
        log "[ERROR]1.generate new images crl fail, stop install"
        rm -rf $crldir/cur_ascend_910.crl
        return 1
    fi
    log "[INFO]1.generate new images crl success, need check if newer"
    #比较cur_ascend_910.crl和ascend_910.crl新旧，输入：类型(0:获取文件，1:比较，2检查有效性，3验签)，路径+new_images，路径+old_images   #输出：返回值，0(SCPS_SAME),1(SCPS_NEW),2(SCPS_OLD),3(SCPS_MIX)
    #如果cur较新，需要做验签，并更新镜像,否则停止安装。验签参数：3，较新的crl，要升级的images
    $crldir/ascend_910_check.bin 1 $crldir/cur_ascend_910.crl $crldir/ascend_910.crl 
    ret=$?
    if [ $ret -eq 0 ] || [ $ret -eq 1 ]; then
         log "[INFO]2.ret=$ret,cur_ascend_910 same or newer, start check cms"
         $crldir/ascend_910_check.bin 3 $crldir/cur_ascend_910.crl $sourcedir/device/tee.bin
         if [ $? -ne 0 ]; then
             log "[ERROR]3.check cur_ascend_910.crl cms failed, stop install"
             rm -rf $crldir/cur_ascend_910.crl
             return 1
         fi
         log "[INFO]3.check cur_ascend_910.crl cms  success, update tool and crl"
         rm -rf $crldir/ascend_910.crl
         mv $crldir/cur_ascend_910.crl $crldir/ascend_910.crl
         if [ $new_tool_flag -eq 0 ]; then
            rm -f $crldir/ascend_910_check.bin
            cp -f $sourcedir/tools/ascend_910_check.bin $crldir/ascend_910_check.bin
            chmod 500 $crldir/ascend_910_check.bin
            cp -f $sourcedir/tools/ascend_upgrade_crl.sh $crldir/ascend_upgrade_crl.sh
            chmod 500 $crldir/ascend_upgrade_crl.sh
            log "[INFO]update ascend_910_check.bin and ascend_upgrade_crl.sh in runpackage"
         fi
         log "[INFO]4.update ascend_910_check.bin and crl success"
    elif [ $ret -eq 2 ] ; then
         log "[INFO]2.ret=$ret,cur_ascend_910 older, start check cms"
         rm -rf $crldir/cur_ascend_910.crl
         $crldir/ascend_910_check.bin 3 $crldir/ascend_910.crl $sourcedir/device/tee.bin
         if [ $? -ne 0 ]; then
             log "[ERROR]3.check ascend_910.crl cms failed,stop install"
             return 1
         fi
         log "[INFO]3.check ascend_910.crl cms  success, not upgrade tool and crl"
    elif [ $ret -eq 5 ] || [ $ret -eq 4 ]; then
        log "[WARNING].ret=$ret,completeness valid check fail, stop drvier install ,need remove crl then upgrade"
        rm -rf $crldir/cur_ascend_910.crl
        return 1
    else
        log "[ERROR]2.ret=$ret,cur_ascend_910 not newer or both crl completeness valid check fail, stop update images"
        rm -rf $crldir/cur_ascend_910.crl
        return 1
    fi
    log "[INFO]case two end: generate new images crl, compare newer and check cms "
    return 0
}
#环境/root/ascend_check里不存在crl，去获取环境上old images的crl,并根据是否存在crl同新的images的crl比较
gen_old_image_crl() {
    log "[INFO]case three start: generate old images crl, then choose to case one or case two"
    if [ ! -e $Driver_Install_Path_Param/driver/device/ascend_910_tee.bin ];then
        #如果old images不存在走首次安装流程，校验新images并判断合法性
        gen_cur_image_crl_check_valid
        if [ $? -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_check_valid failed"
            return 1
        fi
        log "[INFO]gen_cur_image_crl_check_valid success"
        return 0
    fi
    touch $crldir/ascend_910.crl
    chmod 400 $crldir/ascend_910.crl
    log "[INFO]touch ascend_910.crl success"
    if [ -e $crldir/ascend_910_check.bin ]; then
         crltool=$crldir/ascend_910_check.bin
    else
         crltool=$sourcedir/tools/ascend_910_check.bin
    fi
    $crltool 0 $Driver_Install_Path_Param/driver/device/ascend_910_tee.bin $crldir/ascend_910.crl
    if [ $? -ne 0 ]; then
        log "[INFO]old images not exist crl, check new images, choose to case one "
        rm -rf $crldir/ascend_910.crl
        gen_cur_image_crl_check_valid
        if [ $? -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_check_valid failed"
            return 1
        fi
        log "[INFO]gen_cur_image_crl_check_valid success"
        return 0
    else
        log "[INFO]old images exist crl, compare if cur crl newer,choose to case two"
        gen_cur_image_crl_compare_newer
        if [ $? -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_compare_newer failed"
            return 1
        fi
        log "[INFO]gen_cur_image_crl_compare_newer success"
        return 0
    fi
    log "[INFO]case three end: generate old images crl, then choose to case one or case two"
}

device_images_crl_check() {
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$Driver_Install_Path_Param/driver/lib64/common:$Driver_Install_Path_Param/driver/lib64/driver:$Driver_Install_Path_Param/add-ons
    local ret=0
    local first_flag=y
    if [ ! -d $crldir ]; then
        mkdir -p $crldir
        chmod 500 $crldir
        log "[INFO]mkdir $crldir success"
    fi
    if [ -e $driverCrlStatusFile ]; then
        log "[INFO]device images crl have check before, stop check again"
        return 0
    else
        touch $driverCrlStatusFile
    fi

    lsmod | grep "drv_pcie_host " > /dev/null 2>&1 && first_flag=n
    if [ "$first_flag" = "y" ] && [ ! -e $crldir/ascend_910.crl ]; then
        gen_cur_image_crl_check_valid
        if [ $? -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_check_valid failed"
            return 1
        fi
        log "[INFO]first_flag=y,gen_cur_image_crl_check_valid success"
        return 0
    fi
    if [ -e $crldir/ascend_910.crl ]; then
         gen_cur_image_crl_compare_newer
         if [ $? -ne 0 ]; then
            log "[ERROR]gen_cur_image_crl_compare_newer failed"
            return 1
         fi
         log "[INFO]gen_cur_image_crl_compare_newer success"
         return 0
    else
        gen_old_image_crl
        if [ $? -ne 0 ]; then
            log "[ERROR]gen_old_image_crl fail, install failed"
            return 1
        fi
    fi
    return 0
}
