# Docker environment for development of DL workspace
FROM ubuntu:18.04
MAINTAINER Jin Li <jinlmsft@hotmail.com>

ENV DEBIAN_FRONTEND=noninteractive

# Software package installation
RUN apt-get update && apt-get install -y --no-install-recommends \
        apt-utils \
        software-properties-common \
        build-essential \
        cmake \
        git \
        curl \
        wget \
        protobuf-compiler \
        python-dev \
        python-numpy \
        python-pip \
        cpio \
        mkisofs \
        apt-transport-https \
        openssh-client \
        ca-certificates \
        vim \
        sudo \
        git-all \
        sshpass \
        bison \
        libcurl4-openssl-dev libssl-dev \
        dirmngr \
        gnupg-agent

RUN curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -

RUN add-apt-repository \
   "deb [arch=arm64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"

RUN apt-key fingerprint 0EBFCD88

# Install docker
RUN apt-get update && apt-get install -y --no-install-recommends docker-ce

# PIP installation
RUN pip install --upgrade pip && pip install setuptools && pip install pyyaml jinja2 flask flask.restful tzlocal pycurl requests subprocess32

RUN chmod og+rx  -R /usr/local/lib

# RUN curl -sL https://packages.microsoft.com/keys/microsoft.asc | \
#    gpg --dearmor | \
#    sudo tee /etc/apt/trusted.gpg.d/microsoft.asc.gpg > /dev/null

#RUN echo "deb [arch=arm64] https://packages.microsoft.com/repos/azure-cli/ $(lsb_release -cs) main" | \
#    sudo tee /etc/apt/sources.list.d/azure-cli.list

# RUN apt-get update && apt-get install azure-cli




