#!/bin/bash

set -ex
dir=`dirname $0`
aiarts_cm=${dir}/aiarts_cm.yaml

kubectl create configmap aiarts-cm --from-file=${dir}/config.yaml --dry-run -o yaml > ${aiarts_cm}
