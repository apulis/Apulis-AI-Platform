# Copyright (c) Microsoft Corporation
# All rights reserved.
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
# to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

FROM python:3.7 as builder

RUN apt-get update && apt-get --no-install-recommends install -y build-essential git && \
    git clone https://github.com/yadutaf/infilter --depth 1 && \
    cd infilter && make

FROM python:3.7

ARG DCGM_VERSION=1.7.2

RUN curl -SL https://download.docker.com/linux/static/stable/$(uname -m)/docker-19.03.8.tgz \
    | tar -xzvC /usr/local && \
    mv /usr/local/docker/* /usr/bin && \
    apt-get update && apt-get --no-install-recommends install -y iftop lsof \
    ca-certificates wget libgomp1 && \
    mkdir -p /job_exporter && \
    rm -rf /var/lib/apt/lists/*


RUN pip3 install prometheus_client twisted

RUN mkdir /lib64 && cp /lib/ld-linux-aarch64.so.1 /lib64/ld-linux-aarch64.so.1 && cp /usr/local/Ascend/driver/lib64/common/libslog.so /lib/ && \
 cp /usr/local/Ascend/driver/lib64/common/libmmpa.so /lib/ && cp /usr/local/Ascend/driver/lib64/driver/libdrvhdc_host.so /lib/ && \
 cp /usr/local/Ascend/driver/lib64/driver/libdrvdevdrv.so /lib/ && cp /usr/local/Ascend/driver/lib64/driver/libdrvdsmi_host.so /lib/ && \
 cp /usr/local/sbin/npu-smi /usr/local/sbin/npu-smi

RUN wget https://developer.download.nvidia.com/compute/redist/dcgm/${DCGM_VERSION}/DEBS/datacenter-gpu-manager_${DCGM_VERSION}_amd64.deb && \
    dpkg -i datacenter-gpu-manager_*.deb && \
    rm -f datacenter-gpu-manager_*.deb

COPY --from=builder infilter/infilter /usr/bin
COPY src/*.py /job_exporter/
