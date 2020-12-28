#!/bin/sh
ASCEND_SECLOG="/var/log/ascend_seclog"
targetdir=/usr/local/Ascend/driver
sourcedir=$PWD/driver
crldir="/root/ascend_check"
driverCrlStatusFile="/root/ascend_check/driver_crl_status_tmp"

# different product of mini, the subdir is different, there list all of then.
subdir="lib64 host device config boot tools include kernel"
share_dir="lib64 include"
opensource_so="libcrypto.so.1.1 libssl.so.1.1 libibumad.so.3 libibverbs.so.1 librdmacm.so.1 libprotobuf.so.17"

installInfo="/etc/ascend_install.info"
filelist="/etc/ascend_filelist.info"
logFile="${ASCEND_SECLOG}/ascend_install.log"
hotreset_status_file="/var/log/hotreset_status.log"
. $installInfo
username=$UserName
usergroup=$UserGroup
do_dkms=n
no_kernel_flag=n
first_time=y
sed -i "/User=/c User=$username" $sourcedir/tools/ascend_monitor.service
sed -i "/Group=/c Group=$usergroup" $sourcedir/tools/ascend_monitor.service
sed -i "/ExecStart=/c ExecStart=${Driver_Install_Path_Param}/driver/tools/ascend_monitor" $sourcedir/tools/ascend_monitor.service
log() {
    cur_date=`date +"%Y-%m-%d %H:%M:%S"`
    echo "[driver] [$cur_date] "$1 >> $logFile
}

chattrDriver() {
    . $installInfo
    chattr +i $Driver_Install_Path_Param/*.sh > /dev/null 2>&1
    chattr +i $Driver_Install_Path_Param/add-ons/*.so* > /dev/null 2>&1
    chattr -R +i $Driver_Install_Path_Param/driver/* > /dev/null 2>&1
    if [ $installType = "docker" ]; then
        if [ -d $Driver_Install_Path_Param/driver/script ];then
            chattr -R +i $Driver_Install_Path_Param/driver/script/* > /dev/null 2>&1
        fi
        if [ -d $Driver_Install_Path_Param/driver/tools ];then
            chattr -R +i $Driver_Install_Path_Param/driver/tools/* > /dev/null 2>&1
        fi
        if [ -d $Driver_Install_Path_Param/driver/lib64 ];then
            chattr -R +i $Driver_Install_Path_Param/driver/lib64/* > /dev/null 2>&1
        fi
        log "[INFO]add chattr for driver docker mode"
    fi
    log "[INFO]add chattr for driver package"
}

# 更新基础版本号
updateVersionInfoVersion() {
    if [ -f $Driver_Install_Path_Param/driver/version.info ]; then
        rm -f $Driver_Install_Path_Param/driver/version.info
    fi
    cp ./version.info $Driver_Install_Path_Param/driver
    chmod 440 $Driver_Install_Path_Param/driver/version.info
}
# to change files' mode or owner
changeMode(){
    if [ -d "${Driver_Install_Path_Param}" ]; then
        # /etc/hccn.conf
        if [ ! -f "/etc/hccn.conf" ]; then
            touch "/etc/hccn.conf"
        fi
        chmod -f 640 /etc/hccn.conf
        chown -f root:root /etc/hccn.conf

        # /lib/
        chmod -f 440 /lib/davinci.conf
        chown -f ${username}:${usergroup} /lib/davinci.conf


        # driver/kernel
        [ -d ${Driver_Install_Path_Param}/driver/kernel ] && chmod -Rf 500 ${Driver_Install_Path_Param}/driver/kernel

        # hdcd.log
        [ -d /var/log/hdcd ] && chmod -f 640 /var/log/hdcd/hdcd.log

        log "[INFO]some files in /etc/ change success"
    fi
}

# 此函数来自/usr/sbin/dkms中distro_version_rpm
distro_version_rpm()
{
    which rpm > /dev/null 2>&1 || { echo unknown; return; }
    local r wp ver dist

    for r in redhat-release sles-release suse-release ovs-release; do
        wp=$(rpm -q --whatprovides "$r") || continue
        ver=$(rpm -q --qf "%{version}\n" ${wp})
        case $r in
            sles*)
                echo sles${ver}
                ;;
            suse*)
                echo suse${ver}
                ;;
            ovs*)
                echo ovm${ver}
                ;;
            redhat*)
                case $wp in
                    redhat*|sl*)
                        ver=$(echo $ver | \
                        sed -e 's/^\([[:digit:]]*\).*/\1/g')
                        echo el${ver}
                        ;;
                    centos*|enterprise*)
                        echo el${ver}
                        ;;
                    fedora*)
                        echo fc${ver}
                        ;;
                    *)
                        echo unknown
                        ;;
                esac
                ;;
            *)
                echo unknown
                ;;
        esac
        return
    done
    echo unknown
}

# 此函数来自/usr/sbin/dkms中distro_version
distro_version()
{
    # What distribution are we running?
    local LSB_DESCRIPTION DISTRIB_ID DISTRIB_RELEASE ver

    # Try the LSB-provided strings first
    if [ -r /etc/lsb-release ]; then
        . /etc/lsb-release
    elif type lsb_release >/dev/null 2>&1; then
        DISTRIB_ID=$(lsb_release -i -s)
        DISTRIB_RELEASE=$(lsb_release -r -s)
    fi

    case ${DISTRIB_ID} in
        Fedora)
            echo fc${DISTRIB_RELEASE}
            ;;
        RedHatEnterprise*|CentOS|ScientificSL)
            # OEL also reports as such; format is 4.7, 5.3
            ver=$(echo "${DISTRIB_RELEASE}" | \
            sed -e 's/^\([[:digit:]]*\).*/\1/g')
            echo el${ver}
            ;;
        SUSE*)
            if [[ $(lsb_release -d -s) =~ Enterprise ]]; then
                echo sles${DISTRIB_RELEASE}
            else
                echo suse${DISTRIB_RELEASE}
            fi
            ;;
        *)
            if [ ${DISTRIB_ID} ] && [ ${DISTRIB_RELEASE} ]; then
                echo "${DISTRIB_ID}${DISTRIB_RELEASE}"
            else
                distro_version_rpm
            fi
            ;;
    esac
}

# 此函数来自/usr/sbin/dkms中override_dest_module_location
override_dest_module_location()
{
    local orig_location="$1"
    case "$running_distribution" in
    sles[123456789])
        ;;
    suse[123456789])
        ;;
    suse10\.[01])
        ;;
    fc*)
        echo "/extra" && return
        ;;
    el*)
        echo "/extra" && return
        ;;
    ovm*)
        echo "/extra" && return
        ;;
    sles*)
        echo "/updates" && return
        ;;
    suse*)
        echo "/updates" && return
        ;;
    Ubuntu*)
        echo "/updates/dkms" && return
        ;;
    Debian*)
        echo "/updates/dkms" && return
        ;;
    *)
        ;;
    esac
    echo "$orig_location"
}

# 获取dkms输出ko路径
ko_output_absolute_path(){
    running_distribution="`distro_version`"
    local ko_output_path=`override_dest_module_location "/updates"`
    local absolute_path="/lib/modules/`uname -r`${ko_output_path}"
    echo $absolute_path
}

show_path() {
    log "[INFO]target path : $targetdir"
}

update_targetdir() {
    if [ ! -z $1 ] && [ -d $1 ];then
        targetdir=$1
        targetdir=${targetdir%*/}/driver
    fi
}

drv_vermagic_check() {
    pcie_hdc_ko=$sourcedir/host/drv_pcie_hdc_host.ko

    kernel_ver=$(uname -r)
    ko_vermagic=$(modinfo $pcie_hdc_ko | grep vermagic | awk '{print $2}')

    primary_kernel_ver=$(echo $kernel_ver | awk -F [-+] '{print $1}')
    primary_ko_vermagic=$(echo $ko_vermagic | awk -F [-+] '{print $1}')
    if [ "$primary_kernel_ver"x != "$primary_ko_vermagic"x ];then
        log "[ERROR]current kernel primary version($kernel_ver) is different from the driver($ko_vermagic)."
        return 1
    fi

    config_modversions=$(cat /boot/config-$kernel_ver | grep CONFIG_MODVERSIONS)
    log "[INFO]config_modversions is : $config_modversions"
    if [ "$config_modversions"x != "CONFIG_MODVERSIONS=y"x ];then
        # full check vermagic    
        if [ "$kernel_ver"x != "$ko_vermagic"x ];then
            log "[ERROR]driver ko vermagic($ko_vermagic) is different from the os($kernel_ver), need rebuild driver"
            return 1
        fi

        log "[INFO]current kernel version($kernel_ver) is equal to the driver."
    else
        # don't insmod drv_pcie_host.ko, it will be failed when load boot files
        # test insmod the driver
        log "[INFO]drv_pcie_hdc_host.ko might not load, try to insmod it"
        insmod $pcie_hdc_ko >drv_vermagic_check_tmp_log 2>&1
        cat drv_vermagic_check_tmp_log | grep -i "Invalid module format" >>/dev/null
        if [ $? -eq 0 ];then
            log "[ERROR]driver in run packet test load failed, need rebuild driver"
            log "[INFO]test load info :"
            cat drv_vermagic_check_tmp_log >>$logFile
            rm drv_vermagic_check_tmp_log
            return 1
        fi

        rm drv_vermagic_check_tmp_log
        log "[INFO]driver in run packet could load ok, no need rebuild"
    fi

    return 0
}

device_crl_create() {
    if [ -e $crldir/ascend_910.crl ]; then
        ln -sf $crldir/ascend_910.crl $targetdir/device
        log "[INFO]create softlink for crldir/ascend_910.crl"
    fi
}

drv_dkms_dependent_check() {
    log "[INFO]dkms environment check..."

    dpkg-query -s dkms 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
    if [ $? -ne 0 ];then
        rpm -qa 2>/dev/null | grep dkms >/dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[WARNING]The program 'dkms' is currently not installed."
            return 1
        fi
    fi

    dpkg-query -s gcc 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
    if [ $? -ne 0 ];then
        rpm -qa 2>/dev/null | grep gcc >/dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[WARNING]The program 'gcc' is currently not installed."
            return 1
        fi
    fi

    dpkg-query -s linux-headers-$(uname -r) 2>/dev/null | grep "Status: install ok installed" >/dev/null 2>&1
    if [ $? -ne 0 ];then
        rpm -qa 2>/dev/null | grep kernel-headers-$(uname -r) >/dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[WARNING]linux headers is currently not installed."
            return 1
        fi
    fi

    do_dkms=y
    log "[INFO]dkms environment check success"
    return 0
}

drv_dkms_env_check() {
    [ $no_kernel_flag = y ] && return 0
    sources=$sourcedir/kernel

    # to check whether EVB environment.
    if [ ! -d $sources ];then
        drv_vermagic_check
        if [ $? -ne 0 ];then
            log "[ERROR]driver vermagic check failed."
            return 1
        fi
        return 0
    else
        drv_dkms_dependent_check
        if [ $? -ne 0 ];then
            log "[INFO]not detect driver dkms dependence "

            # to check whether drivers' version is equal to kernel.
            drv_vermagic_check
            if [ $? -ne 0 ];then
                log "[ERROR]driver vermagic check failed."
                return 1
            fi
            return 0
        fi
        return 0
    fi
}


load_file_rename() {
    if [ -f $1 ] && [ ! -f $2 ];then
        mv $1 $2
    fi
}

load_file_check() {
    if [ ! -f $1/ascend_910.cpio.gz ] || [ ! -f $1/ascend_910.image ] || [ ! -f $1/ascend_910_tee.bin ];then
        log "[ERROR]host load file is missing, failed"
        return 1
    fi

    return 0
}

device_pcie_load_file_rename() {
    if [ ! -d $targetdir/device ];then
        return 0
    fi

    cd $targetdir/device
    load_file_rename filesystem-le.cpio.gz ascend_910.cpio.gz
    load_file_rename Image ascend_910.image
    load_file_rename tee.bin ascend_910_tee.bin
    cd - >/dev/null
    device_crl_create
    load_file_check $targetdir/device
    if [ $? -ne 0 ];then
        log "[ERROR]load_file_check $targetdir/device failed"
        return 1
    fi

    log "[INFO]device pcie load files rename success"
    return 0
}

filelist_write_one() {
    echo "[$1] $2" >>$filelist
}

filelist_remove() {
    if [ ! -f $filelist ];then
        return 0
    fi

    while read file
    do
        file=$(echo $file | grep "\[$1\]" | awk '{print $2}')
        local tmp_file=${file##*/}
        ls $sys_path | grep $tmp_file > /dev/null 2>&1
        if [ $? -eq 0 ];then
            rm -rf ${sys_path}/${tmp_file}*
        fi

        # remove from filelist
        sed -i 's#'$file'#DAVINCI_FILELIST_DEL#;/DAVINCI_FILELIST_DEL/d' $filelist
    done < $filelist

    return 0
}

chcon_for_selinux(){
    [ $no_kernel_flag = y ] && return 0
    local single_ko
    if getenforce >/dev/null 2>&1;then
        for single_ko in $(ls $targetdir/host/*.ko)
        do
           single_ko=${single_ko##*/}
           chcon -t modules_object_t ${sys_path}/${single_ko}* >>$logFile 2>&1
           chmod 440 ${sys_path}/${single_ko}*
        done
    fi
    log "[INFO]set ko modules_object_t success"
    rm -rf $targetdir/host
    log "[INFO]rm $targetdir/host success."
    return 0
}
driver_auto_insmod() {

    if [ -d $sys_path ];then
        filelist_remove driver_ko
        depmod > /dev/null 2>&1
    else
        mkdir -p $sys_path
        chmod 755 $sys_path
        if [ $? -ne 0 ];then
            log "[ERROR]mkdir -p $sys_path failed"
            return 1
        fi
        log "[INFO]create path: $sys_path success"
    fi

    ls $targetdir/host/ >hiai_ko_file_list_tmp.log

    while read file
    do
        cp -f $targetdir/host/$file $sys_path/$file
    done < hiai_ko_file_list_tmp.log
    rm hiai_ko_file_list_tmp.log

    depmod > /dev/null 2>&1

    log "[INFO]driver ko auto insmod config success"
    return 0
}

driver_ko_install() {
    [ $no_kernel_flag = y ] && return 0
    sys_path=$(ko_output_absolute_path)
    log "[INFO]ko put path $sys_path"
    if [ ! -d $targetdir/host ];then
        log "[ERROR]ko is lost"
        return 1
    fi

    if [ "$do_dkms" = "n" ];then
        driver_auto_insmod
        if [ $? -ne 0 ];then
            log "[ERROR]driver_auto_insmod failed"
            return 1
        fi
    else
        log "[INFO]rebuild the hiai kernel driver by dkms..."
        ./driver/script/run_driver_dkms_install.sh
        if [ $? -ne 0 ];then
            log "[ERROR]dkms install failed"
            return 1
        fi
    fi

    ls $targetdir/host/ >hiai_ko_file_list_tmp.log
    while read file
    do
        filelist_write_one driver_ko $sys_path/$file
    done < hiai_ko_file_list_tmp.log
    rm hiai_ko_file_list_tmp.log

    return 0
}

pcie_load_path_set() {
    if [ ! -d $targetdir/device ];then
        return 0
    fi

    echo DAVINCI_HOME_PATH=${targetdir%/*} > /lib/davinci.conf
    if [ $? -ne 0 ];then
        log "[ERROR]echo DAVINCI_HOME_PATH=${targetdir%/*} > /lib/davinci.conf failed"
        return 1
    fi

    log "[INFO]show /lib/davinci.conf :"`cat /lib/davinci.conf` >>$logFile
    log "[INFO]set /lib/davinci.conf success"

    return 0
}

rand()
{
    min=$1
    max=$(($2-$min+1))
    num=$(cat /dev/urandom | head -n 10 | cksum | awk -F ' ' '{print $1}')
    echo $(($num%$max+$min))
}

rand_mac() {
    mac=$(rand 0 255)
    mac=$(printf "%02x\n" $mac)
    echo $mac
}
pcivnic_mac_conflict_check() {
    conflict=$1
    ifconfig -a | grep "$conflict" >>/dev/null 2>&1
    return $?
}

pcivnic_set_udev_rules() {
    udev_rule_file=/etc/udev/rules.d/70-persistent-net.rules
    ifname=$1
    mac=$2

    if [ ! -f $udev_rule_file ];then
        if [ ! -d /etc/udev/rules.d/ ];then
            mkdir -p /etc/udev/rules.d/
            chmod 755 /etc/udev/rules.d/
        fi
        echo '# This is a sample udev rules file that demonstrates how to get udev to' >>$udev_rule_file
        echo '# set the name of net interfaces to whatever you wish.  There is a' >>$udev_rule_file
        echo '# 16 character limit on network device names though, so do not go too nuts' >>$udev_rule_file
        echo '#' >>$udev_rule_file
        echo '# Note: as of rhel7, udev is case sensitive on the address field match' >>$udev_rule_file
        echo '# and all addresses need to be in lower case.' >>$udev_rule_file
        echo '#' >>$udev_rule_file
        echo '# SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:1f:3c:48:70:b1", ATTR{type}=="1", KERNEL=="eth*", NAME="wlan0"' >>$udev_rule_file
    fi

    # del old
    sed -i 's#'$ifname'#PCIVNIC_TMP_VARIABLE_WILL_DEL#;/PCIVNIC_TMP_VARIABLE_WILL_DEL/d' $udev_rule_file

    # add rule
    echo 'SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="'$mac'", ATTR{type}=="1", KERNEL=="end*", NAME="'$ifname'"' >>$udev_rule_file
}

# mac 10:1b:54:xx:xx:xx is allocated to huawei
pcivnic_mac_set() {
    [ $no_kernel_flag = y ] && return 0
    conf_file=/etc/d-pcivnic.conf
    mac0="10"
    mac1="1b"
    mac2="54"
    mac3=$(rand_mac)
    mac4=$(rand_mac)

    if [ -f $conf_file ];then
        rm $conf_file
    fi

    pcivnic_mac_conflict_check "$mac0:$mac1:$mac2:$mac3:$mac4"
    while [ $? -eq 0 ]
    do
        mac3=$(rand_mac)
        mac4=$(rand_mac)
        pcivnic_mac_conflict_check "$mac0:$mac1:$mac2:$mac3:$mac4"
    done

    echo "# Pcie vnic mac address config file" >>$conf_file
    echo "# Format : device_id mac_address" >>$conf_file

    # mac5 is fixed 0xD3
    mac5="d3"

    dev_id=0
    ifname="endvnic"
    mac_addr="$mac0:$mac1:$mac2:$mac3:$mac4:$mac5"

    echo "$dev_id $mac_addr" >>$conf_file
    pcivnic_set_udev_rules $ifname $mac_addr

    log "[INFO]pcivnic default mac address produce finish"
    return 0
}

driver_file_copy() {
    ./install_common_parser.sh --mkdir --username=$username --usergroup=$usergroup $installType $installPathParam ./filelist.csv
    if [ $? -ne 0 ];then
        log "[ERROR]driver mkdir failed"
        return 1
    fi
    ./install_common_parser.sh --copy --username=$username --usergroup=$usergroup $installType $installPathParam ./filelist.csv
    if [ $? -ne 0 ];then
        log "[ERROR]driver copy failed"
        return 1
    fi
    return 0
}

progress_bar(){
    local parent_progress=$1 weight=$2 child_progress=$3
    local output_progress
    output_progress=`awk 'BEGIN{printf "%d\n",('$parent_progress'+'$weight'*'$child_progress'/100)}'`
    echo "upgradePercentage:${output_progress}%"
    log "[INFO]upgradePercentage:${output_progress}%"
}

is_hot_reset_success()
{
    devnum2=`lspci -D -d 19e5:d801| awk '{print $1}'`
    local bdf
    local pci_dev_path="/sys/bus/pci/devices/"

    bdfs=""
    for bdf in $devnum2
    do
        chip_id=`cat $pci_dev_path$bdf"/chip_id"`
        if [ -e "$chip_id" ];then
            log "[INFO]bdf:$bdf, dev id: "`cat $pci_dev_path$bdf"/dev_id"`", hot reset not finish"
            return 1
        fi
        log "[INFO]bdf:$bdf, dev id: "`cat $pci_dev_path$bdf"/dev_id"`", hot reset finish"
    done

    return 0
}

driver_ko_install_manually() {
    [ "$no_kernel_flag" = "y" ] && return 0
    if [ -e "$hotreset_status_file" ];then
    hotreset_status=`cat "$hotreset_status_file"`
    hotreset_time=${hotreset_status#*.}
    hotreset_status=${hotreset_status%.*}
    else
    hotreset_status="unknown"
    fi
    lsmod | grep "drv_pcie_host " > /dev/null 2>&1 && first_time=n
    log "[INFO]install ko, hot reset status: $hotreset_status, first_time:$first_time"
    if [ "$first_time" = "y" ] || [ "$hotreset_status" = "scan_success" ];then
        ls $sys_path | grep drv_pcie_host.ko > /dev/null 2>&1
        if [ $? -ne 0 ];then
            log "[INFO]$sys_path/drv_pcie_host.ko is not existed,something occurred unexpectly when installing."
            return 1
        fi

        echo "Waiting for device startup..."

        cur_time=$(date +%s)
        if [ "$hotreset_status" = "scan_success" ]; then
            if (($cur_time - $hotreset_time < 10)); then
                sleep_time=$((10 + $hotreset_time - $cur_time))
                log "wait "$sleep_time"s for device bios"
                sleep $sleep_time
            fi
        fi

        # to insmod ko
        ls $sys_path >hiai_get_tmp_ko.log
        while read ko
        do
            ko=${ko%%.*}
            modprobe $ko >/dev/null 2>&1
            log "[INFO] modprobe $ko"
        done < hiai_get_tmp_ko.log
        rm hiai_get_tmp_ko.log

        is_hot_reset_success
        if [ $? -eq 1 ]; then
            echo "Device startup fail"
            hotreset_status="unknown"
        else
            ko_insert_time=$(date +%s)
            if (($ko_insert_time - $cur_time > 200)); then
                echo "Device startup timeout"
                hotreset_status="unknown"
            else
                echo "Device startup success"
            fi
        fi

        sleep 1

        lsmod | grep "drv_pcie_host " >/dev/null
        if [ $? -ne 0 ];then
            log "[ERROR]drv_pcie_host.ko load failed"
            return 1
        fi
        log "[INFO]load driver ko success"

        # startup servers
        if [ ! -f ${targetdir}/../host_sys_init.sh ];then
            log "[ERROR]host_sys_init.sh is not existed, setup servers failed"
            return 1
        fi
        ${targetdir}/../host_sys_stop.sh
        ${targetdir}/../host_sys_init.sh start
        log "[INFO]setup servers success"
        if [ "$hotreset_status" = "scan_success" ]; then
            log "[INFO]hot reset success"
            echo "success" > "$hotreset_status_file"
        fi
    fi

    return 0
}

install_tool() {
    if [ -f $targetdir/script/run_driver_tool_install.sh ]; then
        $targetdir/script/run_driver_tool_install.sh
        if [ $? -ne 0 ]; then
            log "[ERROR]tool install failed."
            return 1
        fi
    else
        log "[WARNING]tool install script not found."
    fi
    return 0
}

# start!
installPathParam=$1
installType=$2

no_kernel_flag=n
percent=0
weight=100

update_targetdir $installPathParam
if [ $installType = "docker" ]; then
    no_kernel_flag=y
    log "[INFO]no_kernel_flag value is [$no_kernel_flag]"
fi

show_path

drv_dkms_env_check
if [ $? -ne 0 ];then
    echo -e "\033[31m drv_dkms_env_check failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi
progress_bar $percent $weight 10

driver_file_copy
if [ $? -ne 0 ];then
    echo -e "\033[31m drv_file_copy failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi
progress_bar $percent $weight 30

#check CRL of images
if [ $no_kernel_flag = n ]; then
    . ./driver/script/device_crl_check.sh
    device_images_crl_check
    if [ $? -ne 0 ];then
        echo -e "\033[31m device_images_crl_check, details in : $logFile \033[0m"
        log "[ERROR]cur device crl check fail, stop upgrade"
        if [ -e $driverCrlStatusFile ]; then
            rm -f $driverCrlStatusFile
            log "[INFO]rm -f $driverCrlStatusFile success"
        fi
        exit 1
    else
        log "[INFO]cur device crl check success"
        if [ -e $driverCrlStatusFile ]; then
            rm -f $driverCrlStatusFile
            log "[INFO]rm -f $driverCrlStatusFile success"
        fi
    fi
fi
# device load files rename
device_pcie_load_file_rename
if [ $? -ne 0 ];then
    echo -e "\033[31m device_pcie_load_file_rename failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi

# save install path to /lib/davinci.conf 
pcie_load_path_set
if [ $? -ne 0 ];then
    echo -e "\033[31m pcie_load_path_set failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi
progress_bar $percent $weight 40

# drvier ko auto insmod
driver_ko_install
if [ $? -ne 0 ];then
    echo -e "\033[31m driver_ko_install failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi

#to set driver/host/*.ko selinux
chcon_for_selinux

# to set boot-items for daemons.
${targetdir}/../host_servers_setup.sh
if [ $? -ne 0 ]; then
    echo -e "\033[31m set boot-items failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi

progress_bar $percent $weight 90
# config pcie vnic mac address
pcivnic_mac_set
changeMode
install_tool

# if kos aren't in kernel, it will insmod them manually
driver_ko_install_manually
if [ $? -ne 0 ];then
    echo -e "\033[31m driver_ko_install_manually failed, details in : $logFile \033[0m"
    changeMode
    exit 1
fi
updateVersionInfoVersion
chattrDriver
progress_bar $percent $weight 100
exit 0
