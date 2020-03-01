# Copyright 2020 Apulic, Inc.  All rights reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

#### Docker file for tag:  apulis/npu-test:devel

FROM ubuntu:18.04
MAINTAINER Andrew Su <andrew.su@apulis.com>
RUN apt update \
 && apt upgrade -y \
 && apt install -y git bash python3 python3-pip \
 && pip3 install --upgrade pip

# && pip3 install tensorflow==1.12

WORKDIR /root/

ADD tensorflow-1.12.0-cp36-cp36m-linux_x86_64.whl .

RUN pip3 install tensorflow-1.12.0-cp36-cp36m-linux_x86_64.whl

COPY code /d_solution
WORKDIR /d_solution/

