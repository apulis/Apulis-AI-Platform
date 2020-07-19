#/bin/bash

os_name() {
    if [ ! -f $1 ];then
        HostOsName=unknown
        HostOsVersion=unknown
        return
    fi

    HostOsName=$(cat $1 | grep ^NAME= | awk -F "[\" ]" '{print $2}' | awk '{print $1}')
    if [ x"$HostOsName" = "x" ];then
        HostOsName=unknown
    fi
    HostOsVersion=$(cat $1 | grep ^VERSION_ID= | awk -F "\"" '{print $2}')
    if [ x"$HostOsVersion" = "x" ];then
        HostOsVersion=unknown
    fi
    return
}

get_os_info() {
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
    return
}

get_system_info() {
    get_os_info
    HostArch=$(uname -m)
    KernelVersion=$(uname -r)
}
