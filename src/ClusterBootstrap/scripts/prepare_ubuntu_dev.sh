#!/bin/bash

set -x

install_python() {

    sudo apt-get update -y
    sudo apt-get install -y python
    sudo apt-get install -y python-jinja2
    sudo apt-get install -y python-yaml
    sudo apt-get install -y python-requests
    sudo apt-get install -y python-subprocess32
    sudo apt-get install -y python-tzlocal
    sudo apt-get install -y python-pycurl
    sudo apt install -y python-pip
    sudo pip install -U PyYAML
    sudo apt-get install -y sshpass
}

install_docker() {
    sudo apt-get install \
        apt-transport-https \
        ca-certificates \
        curl \
        gnupg-agent \
        software-properties-common

    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

    if [ `arch`="aarch64" ]; then
	echo "arch is arm64"
    	sudo add-apt-repository \
   	    "deb [arch=arm64] https://download.docker.com/linux/ubuntu \
   		$(lsb_release -cs) \
   		stable"
    else
	echo "arch is amd64"
        sudo add-apt-repository \
	    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
	        $(lsb_release -cs) \
		stable"
    fi 

    sudo apt-get install docker-ce docker-ce-cli containerd.io -y
}


main () {
    install_python
    install_docker
}

main $*

