FROM python:3.7-slim

WORKDIR /

RUN apt update && apt install -y gcc libffi-dev git
RUN git clone -b 0.2.2 https://github.com/kubeflow/kfserving.git

RUN pip install --upgrade pip && pip install /kfserving/python/kfserving

RUN chmod +x /kfserving/python/storage-initializer/scripts/initializer-entrypoint
ENTRYPOINT ["/kfserving/python/storage-initializer/scripts/initializer-entrypoint"]