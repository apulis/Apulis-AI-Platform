FROM horovod/horovod:0.18.1-tf1.14.0-torch1.2.0-mxnet1.5.0-py3.5
MAINTAINER Jin Li <jinlmsft@hotmail.com>

# RUN rm -rf /var/lib/apt/lists/*
# Add network tools for debug
RUN umask 022
RUN apt-get -y update && \
    apt-get -y install \
      vim \
      wget \
      curl \
      jq \
      gawk \
      openssh-client \
      git \
      rsync \
      sudo \
      zip \
      unzip \
      openssh-server \
      sudo \
      nodejs \
      software-properties-common \
      net-tools \
      dnsutils \
      openssh-server \
      sudo 

RUN echo $'# Add by Dockerfile\n\
AddressFamily inet\n\
AllowTcpForwarding yes\n\
X11UseLocalhost yes\n\
' > /etc/ssh/sshd_config

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        git \
        wget \
        vim \
        locales \
        curl \
        apt-transport-https \
        screen \
        htop      


RUN mkdir -p /var/www/html/
RUN apt-get update -y && apt-get install -y python3-pycurl locales ssh-askpass  python3-tk screen python3-pip
RUN pip3 install --upgrade pip 
RUN pip3 install setuptools 
RUN pip3 install flask flask.restful 
RUN pip3 install bs4 requests 
RUN pip3 install -U flask-cors
# RUN pip3 uninstall -y numpy && pip install numpy==1.14.5
RUN pip3 install pandas jupyter matplotlib spacy sklearn scikit-image munch imgaug
RUN python3 -m spacy download en

# RUN sudo npm install bower -g 


ENV APACHE_RUN_USER www-data
ENV APACHE_RUN_GROUP www-data
ENV APACHE_LOG_DIR /var/log/apache2

#EXPOSE 80
#EXPOSE 5000

RUN chmod -R 755 /var/www/html/

COPY wwwroot /var/www/html

# netcore 2.2
RUN add-apt-repository ppa:graphics-drivers/ppa
RUN wget -q https://packages.microsoft.com/config/ubuntu/16.04/packages-microsoft-prod.deb
RUN sudo dpkg -i packages-microsoft-prod.deb
RUN apt-get update -y && \
      apt-get install -y dotnet-sdk-2.2 apache2 libapache2-mod-wsgi-py3 
# RUN apt-get update -y && \
#      apt-get install -y apache2 libapache2-mod-wsgi-py3 

RUN rm /etc/apache2/mods-enabled/mpm_*
COPY mpm_prefork.conf /etc/apache2/mods-available/mpm_prefork.conf
COPY 000-default.conf /etc/apache2/sites-available/000-default.conf
COPY ports.conf /etc/apache2/ports.conf
RUN ln -s /etc/apache2/mods-available/mpm_prefork.conf /etc/apache2/mods-enabled/mpm_prefork.conf
RUN ln -s /etc/apache2/mods-available/mpm_prefork.load /etc/apache2/mods-enabled/mpm_prefork.load
COPY bingserver-restfulapi.wsgi /wsgi/bingserver-restfulapi.wsgi
ADD RestAPI /BingServer/src/RestAPI

#ADD utils /BingServer/src/RestAPI/utils
RUN echo "ServerName localhost" >> /etc/apache2/apache2.conf
ADD run.sh /run.sh
RUN chmod +x /run.sh


RUN mkdir -p /var/log/apache2
RUN chmod 0777 /var/log/apache2
RUN chmod -R 0777 /root

# # RUN apt-get update -y && apt-get install ssh-askpass rssh molly-guard ufw monkeysphere python3-ndg-httpsclient -y

# ENV LD_LIBRARY_PATH /usr/local/nvidia/lib64/

EXPOSE 1380 180
ADD RecogServer /RecogServer

# Install jupyter lab, add jupyter2 kernel
RUN pip3 install jupyterlab
RUN python3 -m pip install ipykernel
RUN python3 -m ipykernel install --user

#Telemetry
#--------------
#The .NET Core tools collect usage data in order to improve your experience. The data is anonymous and does not include command-line arguments. The data is collected by Microsoft and shared with the community.
#You can opt out of telemetry by setting a DOTNET_CLI_TELEMETRY_OPTOUT environment variable to 1 using your favorite shell.
#You can read more about .NET Core tools telemetry @ https://aka.ms/dotnet-cli-telemetry.
ENV DOTNET_CLI_TELEMETRY_OPTOUT 1
#WORKDIR /var/www/html
#RUN bower --allow-root install ng-file-upload --save

# Need to run privileged mode
# python /root/certificate-service/genkey-restapi.py && 

# CMD /bin/bash -c "service apache2 start && sleep infinity"

CMD /bin/bash -c /run.sh
