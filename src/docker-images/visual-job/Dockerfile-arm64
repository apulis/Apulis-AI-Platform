FROM ubuntu:18.04
RUN apt-get update -y
RUN umask 022
RUN apt-get install -y python3-pip
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip config set global.index-url https://mirrors.aliyun.com/pypi/simple/
RUN python3 -m pip install tensorboard
RUN mkdir -p /data/tensorborad-log
CMD ["/bin/sh" "-c" "sleep infinity"]