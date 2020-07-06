FROM ubuntu:18.04
MAINTAINER Jin Li <jin.li@apulis.com>

RUN apt-get update -y; apt-get install -y --no-install-recommends apt-transport-https \
        build-essential \
        cmake \
        git \
        wget \
        vim \
        python-dev \
        python-pip \
        python-yaml \ 
        locales \
        python-pycurl \
        bison \
        curl \
        nfs-common \
        apt-utils \
        screen \
        htop \
        libmysqlclient-dev \
        unixodbc-dev gcc g++ \
        ssh apache2 libapache2-mod-wsgi sudo


RUN pip install --upgrade pip; 
RUN pip install setuptools; 
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8
RUN pip install flask
RUN pip install flask.restful
RUN pip install requests mysql-connector-python pyodbc tzlocal faulthandler

RUN usermod -a -G sudo www-data 
RUN echo "\nwww-data ALL=(ALL) NOPASSWD:ALL\n" > /etc/sudoers
