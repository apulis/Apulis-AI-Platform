FROM qianjiangyuan/tutorial-horovod:1.7
MAINTAINER Jin Li <jinlmsft@hotmail.com>

###################
RUN apt-get update && apt-get install -y --no-install-recommends \
    	protobuf-compiler \
      	build-essential \
        curl \
        git \
		cmake \
        rsync \
        software-properties-common \
        unzip \
        zip \
        libcurl3-dev \
        libfreetype6-dev \
        libpng12-dev \
        libzmq3-dev \
        pkg-config \
        python-dev \
        zlib1g-dev \
        libopencv-dev \
        python-opencv \
		python-tk \
        build-essential autoconf libtool libcunit1-dev \
        libproj-dev libgdal-dev libgeos-dev libjson0-dev vim python-gdal \
    	# requirements for keras
    	python-h5py \
    	python-yaml \
    	python-pydot \
		graphviz \
        && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

###################
# python packages
RUN curl -fSsL -O https://bootstrap.pypa.io/get-pip.py && \
    python get-pip.py && \
    rm get-pip.py
RUN pip --no-cache-dir install \
		protobuf \
		ipykernel \
    	jupyter \
    	matplotlib \
    	numpy \
    	scipy \
    	sklearn \
		scikit-image \
		pandas \
		h5py \
		shapely \
    	&& \
    python -m ipykernel.kernelspec


###################
# object detection api
# https://github.com/phipleg/tensorflow-object-detector/blob/master/Dockerfile
# git clone --branch my_abc http://git.abc.net/git/abc.git

RUN git clone --branch r1.5 https://github.com/tensorflow/models.git /opt/tensorflow-models
WORKDIR /opt/tensorflow-models/research
RUN pip install -e .
RUN protoc object_detection/protos/*.proto --python_out=.
ENV PYTHONPATH $PYTHONPATH:/opt/tensorflow-models:/opt/tensorflow-models/slim
ENV PYTHONPATH $PYTHONPATH:/opt/tensorflow-models:/opt/tensorflow-models/research/slim
# test (do this after container has been created)?
#RUN python /opt/tensorflow-models/research/object_detection/builders/model_builder_test.py

RUN pip install jupyterlab
RUN python -m pip install ipykernel
RUN python -m ipykernel install --user

###################
# Set up our notebook config.
WORKDIR /raid/
# TensorBoard
RUN ["/bin/bash"]