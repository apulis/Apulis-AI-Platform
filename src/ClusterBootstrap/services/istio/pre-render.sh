#!/bin/bash

if [ "$use_service" = "istio" ];
then
    if ! [ -x "$(command -v istioctl)" ] ; then
	  curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 sh -
    export PATH=$PWD/istio-1.6.8/bin/:$PATH
    fi
    istioctl install -f services/istio/istio.yaml --set values.global.jwtPolicy=first-party-jwt --force
fi