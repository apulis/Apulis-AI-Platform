FROM ubuntu:18.04
RUN  apt-get update

RUN apt-get install -y apt-transport-https ca-certificates curl gnupg2 software-properties-common git  && \
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add - && \
    add-apt-repository "deb [arch=arm64] https://mirrors.tuna.tsinghua.edu.cn/docker-ce/linux/ubuntu $(lsb_release -cs) stable"  && \
    apt-get update && \
    apt-get install docker-ce-cli
RUN apt-get -y install python-mysqldb build-essential python-dev libmysqlclient-dev python-pip net-tools libcurl4-openssl-dev
RUN apt-get -y build-dep python-mysqldb
RUN curl -LO "https://storage.googleapis.com/kubernetes-release/release/$(curl -s https://storage.googleapis.com/kubernetes-release/release/stable.txt)/bin/linux/amd64/kubectl" && \
    chmod +x ./kubectl && \
    cp kubectl /usr/bin/
RUN mkdir /root/workspace
RUN pip install --upgrade pip -i https://mirrors.aliyun.com/pypi/simple/ && \
    pip config set global.index-url https://mirrors.aliyun.com/pypi/simple/ && \
    pip config set install.trusted-host mirrors.aliyun.com
WORKDIR /root/workspace
