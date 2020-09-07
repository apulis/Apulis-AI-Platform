#!/bin/bash

if [ "$use_service" = "istio" ];
then
    if ! [ -x "$(command -v istioctl)" ] ; then
	    curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 sh -
	    cp istio-1.6.8/bin/istioctl /usr/bin/
    fi
    if [ "$(uname -m)" = "aarch64" ]; then
      istioctl install -f services/istio/istio-arm64.yaml --set values.global.jwtPolicy=first-party-jwt --force
    else
      istioctl install -f services/istio/istio.yaml --set values.global.jwtPolicy=first-party-jwt --force
    fi
fi