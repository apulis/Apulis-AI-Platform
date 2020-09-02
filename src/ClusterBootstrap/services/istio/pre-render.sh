#!/bin/bash

if [ "$use_service" = "istio" ];
then
	  curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 sh -
    istio-1.6.8/bin/istioctl install -f services/istio/istio.yaml --set values.global.jwtPolicy=first-party-jwt --force
else
  echo $use_service
fi