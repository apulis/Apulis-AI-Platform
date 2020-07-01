FROM ubuntu:18.04
MAINTAINER Jin Li <jinlmsft@hotmail.com>

# see https://github.com/phusion/baseimage-docker/issues/58
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

#RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
#RUN echo "deb http://download.mono-project.com/repo/debian wheezy main" | tee /etc/apt/sources.list.d/mono-xamarin.list
#RUN echo "deb http://download.mono-project.com/repo/debian wheezy-apache24-compat main" | tee -a /etc/apt/sources.list.d/mono-xamarin.list
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        git \
        wget \
        vim \
        python3-dev \
        python3-numpy \
        python3-pip \
        python3-yaml \
        locales \
        curl \
        apt-transport-https 

#RUN sh -c 'echo "deb [arch=amd64] https://apt-mo.trafficmanager.net/repos/dotnet-release/ xenial main" > /etc/apt/sources.list.d/dotnetdev.list'
#RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 417A0893
#RUN apt-get install -y apt-transport-https
#RUN apt-get update
#RUN apt-get install -y dotnet-dev-1.0.4

# netcore 2.0
RUN wget -q https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb
RUN dpkg -i packages-microsoft-prod.deb
RUN apt-get update
RUN apt-get install -y dotnet-sdk-2.2
RUN apt-get install -y aspnetcore-runtime-2.1

RUN apt-get install -y bison curl nfs-common

# Get pip 
# RUN wget https://bootstrap.pypa.io/get-pip.py
# RUN python get-pip.py
RUN pip3 install --upgrade pip; 

RUN pip3 install setuptools; 
RUN locale-gen en_US.UTF-8
RUN update-locale LANG=en_US.UTF-8

RUN pip3 install flask
RUN pip3 install flask.restful

#this ssh key is used to download DLWorkspace from github. It has read-only access to github repo. 
RUN apt-get install -y --no-install-recommends ssh
RUN apt-get install -y software-properties-common 
RUN add-apt-repository ppa:certbot/certbot
RUN apt-get update; apt-get install -y certbot

#Telemetry
#--------------
#The .NET Core tools collect usage data in order to improve your experience. The data is anonymous and does not include command-line arguments. The data is collected by Microsoft and shared with the community.
#You can opt out of telemetry by setting a DOTNET_CLI_TELEMETRY_OPTOUT environment variable to 1 using your favorite shell.
#You can read more about .NET Core tools telemetry @ https://aka.ms/dotnet-cli-telemetry.
ENV DOTNET_CLI_TELEMETRY_OPTOUT 1

#this ssh key is used to download DLWorkspace from github. It has read-only access to github repo. 
RUN apt-get install -y --no-install-recommends ssh

WORKDIR /usr/WebPortal

ADD WebPortal /usr/WebPortal
COPY run.sh .
RUN chmod +x run.sh
CMD ./run.sh