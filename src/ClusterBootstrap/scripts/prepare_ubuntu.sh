#!/bin/bash

set -x

# https://unix.stackexchange.com/questions/146283/how-to-prevent-prompt-that-ask-to-restart-services-when-installing-libpq-dev
export DEBIAN_FRONTEND=noninteractive

# 用于记录安装进度
install_dir="/tmp/dlws_install/"

# 用于标记是否为reboot之后，继续安装的
exec_mode="from_begging"


# 预安装
install_preparation() {

    sudo killall apt-get
    sudo killall dpkg
    sudo dpkg --configure -a
}

# Install python on CoreOS base image
# Docker environment for development of DL workspace
install_python() {

    ## 检查本步骤是否已安装过
    if installed "install_python";
    then
        return 1
    fi

    sudo apt-get update -y
    yes | sudo apt-get install -y --no-install-recommends \
            apt-utils \
            software-properties-common \
            build-essential \
            cmake \
            git \
            curl \
            wget \
            python-dev \
            python-pip \
            python-yaml \
            python-jinja2 \
            python-argparse \
            python-setuptools \
            apt-transport-https \
            ca-certificates \
            vim \
            sudo \
            nfs-common \
            dos2unix

    ## 设置安装记录
    set_install_flag "install_python"
}

install_shell_utils() {

    ## 检查本步骤是否已安装过
    if installed "install_shell_utils";
    then
        return 1
    fi

    yes | sudo apt-get install -y bison curl parted
    set_install_flag "install_shell_utils"
}
    
install_docker() {

    ## 检查本步骤是否已安装过
    if installed "install_docker";
    then
        return 1
    fi

    # Install docker
    which docker
    if [ $? -eq 0 ]
    then 
        docker --version
        ## docker already installed
    else
        sudo apt-get remove docker docker-engine docker.io
        curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
        sudo add-apt-repository \
        "deb [arch=amd64] https://mirrors.aliyun.com/docker-ce/linux/ubuntu \
        $(lsb_release -cs) \
        stable"
        sudo apt-get update
        yes | sudo apt-get install -y docker-ce

        ## 增加国内数据源
        sudo rm -f /etc/docker/daemon.json
        if  lspci | grep -qE "[0-9a-fA-F][0-9a-fA-F]:[0-9a-fA-F][0-9a-fA-F].[0-9] (3D|VGA compatible) controller: NVIDIA Corporation.*" ;
        then 
            ## gpu驱动
            sudo echo '
                {
                    "registry-mirrors": [
                        "https://registry.docker-cn.com"
                    ],
                    "insecure-registries": [],
                    "runtimes": {
                        "nvidia": {
                            "path": "nvidia-container-runtime",
                            "runtimeArgs": []
                        }
                    }
                }
            ' > /etc/docker/daemon.json
        else
            sudo echo '
                {
                    "registry-mirrors": [
                        "https://registry.docker-cn.com"
                    ],
                    "insecure-registries": []
                }
            ' > /etc/docker/daemon.json
        fi
    fi

    yes | sudo pip install --upgrade pip
    # pip doesn't install python for root account, causing issues. 
    # sudo pip install setuptools
    # sudo pip install pyyaml jinja2 argparse

    sudo usermod -aG docker $USER
    set_install_flag "install_docker"
}

set_network() {
    ## 检查本步骤是否已安装过
    if installed "set_network";
    then
        return 1
    fi

    if [ -f /etc/NetworkManager/NetworkManager.conf ]; then
            sed "s/^dns=dnsmasq$/#dns=dnsmasq/" /etc/NetworkManager/NetworkManager.conf > /tmp/NetworkManager.conf && sudo mv /tmp/NetworkManager.conf /etc/NetworkManager/NetworkManager.conf
            sudo service network-manager restart
    fi

    sudo service apache2 stop
    set_install_flag "set_network"
}

install_gpu_utils() {

    ## Check if this node has gpu
    if  lspci | grep -qE "[0-9a-fA-F][0-9a-fA-F]:[0-9a-fA-F][0-9a-fA-F].[0-9] (3D|VGA compatible) controller: NVIDIA Corporation.*" ; 
    then

        ## install from the beginning
        if ! resume_mode;
        then
            # https://askubuntu.com/questions/481414/install-nvidia-driver-instead-of-nouveau
            # Start from 10/05/2017 the following is needed. 
            if ! grep -q "blacklist nouveau" -F /etc/modprobe.d/blacklist.conf; then 
                    echo "blacklist vga16fb" | sudo tee --append /etc/modprobe.d/blacklist.conf > /dev/null
                    echo "blacklist nouveau" | sudo tee --append /etc/modprobe.d/blacklist.conf > /dev/null
                    echo "blacklist rivafb" | sudo tee --append /etc/modprobe.d/blacklist.conf > /dev/null
                    echo "blacklist nvidiafb" | sudo tee --append /etc/modprobe.d/blacklist.conf > /dev/null
                    echo "blacklist rivatv" | sudo tee --append /etc/modprobe.d/blacklist.conf > /dev/null
                    sudo apt-get remove -y --purge nvidia-*
                    sudo update-initramfs -u
            fi

            #       NVIDIA_VERSION=384.98
            #       # make the script reexecutable after a failed download
            #       rm /tmp/NVIDIA-Linux-x86_64-$NVIDIA_VERSION.run
            #       wget -P /tmp http://us.download.nvidia.com/XFree86/Linux-x86_64/$NVIDIA_VERSION/NVIDIA-Linux-x86_64-$NVIDIA_VERSION.run
            #       chmod +x /tmp/NVIDIA-Linux-x86_64-$NVIDIA_VERSION.run
            #       sudo bash /tmp/NVIDIA-Linux-x86_64-$NVIDIA_VERSION.run -a -s
            sudo systemctl stop kubelet

            echo kill all containers so we could remove old nvidia drivers
            timeout 10 docker kill $(docker ps -a -q)

            lsmod | grep -qE "^nvidia" &&
                {
                    echo ======== NVIDIA driver is running, uninstall it =========
                    DEP_MODS=`lsmod | tr -s " " | grep -E "^nvidia" | cut -f 4 -d " "`
                    for mod in ${DEP_MODS//,/ }
                    do
                        sudo rmmod $mod ||
                            {
                                echo "The driver $mod is still in use, can't unload it."
                                exit 1
                            }
                    done
                    sudo rmmod nvidia ||
                        {
                            echo "The driver nvidia is still in use, can't unload it."
                            exit 1
                        }
                }

            sudo add-apt-repository -y ppa:graphics-drivers/ppa
            sudo apt-get purge -y nvidia*
            sudo apt-get update
            yes | sudo apt-get install -y nvidia-driver-430

            show_continue_msg 
            sudo shutdown -r
            return 0

        ## resume the installation, 
        ## usually it's done after a reboot
        else
            yes | sudo apt install -y nvidia-modprobe
            sudo rm -r /opt/nvidia-driver || true

            # Install nvidia-docker and nvidia-docker-plugin ( Upgrade to nvidia-docker2)
            # rm /tmp/nvidia-docker*.deb
            # wget -P /tmp https://github.com/NVIDIA/nvidia-docker/releases/download/v1.0.1/nvidia-docker_1.0.1-1_amd64.deb
            # sudo dpkg -i /tmp/nvidia-docker*.deb && rm /tmp/nvidia-docker*.deb

            curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add -
            distribution=$(. /etc/os-release;echo $ID$VERSION_ID)
            curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list
            sudo apt-get update

            yes | sudo apt-get install -y nvidia-docker2
            sudo pkill -SIGHUP dockerd

            # Test nvidia-smi
            sudo nvidia-docker run --rm dlws/cuda nvidia-smi


            sudo mkdir -p /opt/nvidia-driver/
            sudo cp -r /var/lib/nvidia-docker/volumes/nvidia_driver/* /opt/nvidia-driver/
            NVIDIA_VERSION=`/usr/bin/nvidia-smi -x -q | grep driver_version | sed -e 's/\t//' | sed -e 's/\ //' | sed -e 's/<driver_version>//' | sed -e 's/<\/driver_version>//'`
            NV_DRIVER=/opt/nvidia-driver/$NVIDIA_VERSION
            sudo ln -s $NV_DRIVER /opt/nvidia-driver/current
        fi
    fi

    set_install_flag "install_shell_utils"
}


set_kubernetes() {

    # https://github.com/kubernetes/kubeadm/issues/610
    sudo swapoff -a
    set_install_flag "set_kubernetes"
}

set_azure() {

    # Install azure blobfuse
    sudo rm -f packages-microsoft-prod.deb
    wget https://packages.microsoft.com/config/ubuntu/16.04/packages-microsoft-prod.deb
    sudo dpkg -i packages-microsoft-prod.deb
    sudo apt-get update
    sudo apt-get install -y blobfuse fuse jq
    sudo rm -f packages-microsoft-prod.deb
}

resume_mode() {
    if [ "$exec_mode" = "continue" ]
    then
        true
    else 
        false
    fi
}

set_install_flag() {
    if [ -n $1 ]
    then
        sudo mkdir -p "${install_dir}"
        sudo touch "${install_dir}/$1"
    fi 
}

del_install_flag() {
    if [ -n $1 ]
    then
        sudo rm -f "${install_dir}/$1"
    fi 
}

installed() {
    if [ -n $1 -a -f "${install_dir}/$1" ]
    then
        true
    else
        false
    fi 
}

# 删除安装标记位
purge_install_flags() {
    echo "purge_install_flags: $*"

    # 只在重新安装时，才移除旧的安装进度
    if ! resume_mode;
    then 
        del_install_flag  "install_preparation"
        del_install_flag  "install_python"
        del_install_flag  "install_shell_utils"
        del_install_flag  "install_docker"
        del_install_flag  "set_network"
        del_install_flag  "install_gpu_utils"
        del_install_flag  "set_kubernetes"

        #echo "in resume mode: $1"
    fi
}

show_continue_msg() {
    echo "please re-execute the command ending with \"continue\"!!!!!"
}

main () {

    exec_mode=$1
    echo "exec_mode: $exec_mode"

    purge_install_flags
    install_preparation
    install_python
    install_shell_utils
    install_docker
    set_network
    install_gpu_utils
    set_kubernetes
    set_azure
}

main $*

