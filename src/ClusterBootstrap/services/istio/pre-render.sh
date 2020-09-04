#!/bin/bash

if [ "$use_service" = "istio" ];
then
    export PATH=$PWD/istio-1.6.8/bin/:$PATH
    if ! [ -x "$(command -v istioctl)" ] ; then
	  curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 sh -
    fi
    if [ "$(uname -m)" = "aarch64" ]; then
      istioctl install -f services/istio/istio-arm64.yaml --set values.global.jwtPolicy=first-party-jwt --force
    else
      istioctl install -f services/istio/istio.yaml --set values.global.jwtPolicy=first-party-jwt --force
    fi
fi