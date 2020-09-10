#!/bin/bash

if [ "$use_service" = "knative" ];
then
    dir=`dirname $0`
    ca_file_name=${dir}/ca.yaml
    kubectl --namespace knative-serving create secret generic customca --from-file=customca.crt=/etc/docker/certs.d/harbor.sigsus.cn:8443/ca.crt --dry-run -o yaml > $ca_file_name
fi