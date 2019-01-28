FROM qianjiangyuan/tutorial-horovod:1.7
MAINTAINER Jin Li <jinlmsft@hotmail.com>

RUN git clone https://github.com/NVIDIA/apex.git && cd apex && python setup.py install --cuda_ext --cpp_ext

RUN pip install pytorch-pretrained-bert pytest