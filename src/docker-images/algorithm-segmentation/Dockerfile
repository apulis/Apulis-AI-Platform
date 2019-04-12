FROM qianjiangyuan/tutorial-horovod:1.8

MAINTAINER Jin Li <jinlmsft@hotmail.com>

###################
RUN update-ca-certificates
RUN apt-get update -y && apt-get install -y --no-install-recommends \
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
        python3.5-dev \
        zlib1g-dev \
        libopencv-dev \
        # python3-opencv \
		python3-tk \
        build-essential autoconf libtool libcunit1-dev \
        libproj-dev libgdal-dev libgeos-dev libjson0-dev vim python3-gdal \
    	# requirements for keras
    	python3-h5py \
    	python3-yaml \
    	python3-pydot \
        python3-pip \
        python3-setuptools \
		graphviz 

###################
# python packages
RUN pip3 install --upgrade pip
RUN pip3 --no-cache-dir install \
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
        opencv-python \
    	&& \
    python3 -m ipykernel.kernelspec

###################
# Install TensorFlow GPU version.
#pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-0.12.1-cp27-none-linux_x86_64.whl
#pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow_gpu-1.4.0-cp27-none-linux_x86_64.whl
# ARG TENSORFLOW_VERSION=1.4.1
# ARG TENSORFLOW_DEVICE=gpu
# ARG TENSORFLOW_APPEND=_gpu
# RUN pip3 --no-cache-dir install https://storage.googleapis.com/tensorflow/linux/${TENSORFLOW_DEVICE}/tensorflow${TENSORFLOW_APPEND}-${TENSORFLOW_VERSION}-cp35-none-linux_x86_64.whl
RUN pip3 --no-cache-dir install tensorflow-gpu keras


###################
# keras
# ARG KERAS_VERSION=2.0.6
# ENV KERAS_BACKEND=tensorflow
# RUN pip3 --no-cache-dir install --no-dependencies git+https://github.com/fchollet/keras.git@${KERAS_VERSION}

###################
# mxnet
RUN pip3 --no-cache-dir install mxnet-cu80mkl


###################
# object detection api
# https://github.com/phipleg/tensorflow-object-detector/blob/master/Dockerfile
# git clone --branch my_abc http://git.abc.net/git/abc.git

RUN git clone --branch r1.5 https://github.com/tensorflow/models.git /opt/tensorflow-models
RUN cd /opt/tensorflow-models/research && \
    pip3 install -e . && \
    protoc object_detection/protos/*.proto --python_out=. 

ENV PYTHONPATH $PYTHONPATH:/opt/tensorflow-models:/opt/tensorflow-models/slim
ENV PYTHONPATH $PYTHONPATH:/opt/tensorflow-models:/opt/tensorflow-models/research/slim
# test (do this after container has been created)?
#RUN python /opt/tensorflow-models/research/object_detection/builders/model_builder_test.py

RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install jupyter

RUN pip3 install jupyterlab
RUN python3 -m pip install ipykernel
RUN python3 -m ipykernel install --user

###################
# TensorBoard
