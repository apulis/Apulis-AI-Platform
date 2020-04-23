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


main () {
    install_python
}

main $*

