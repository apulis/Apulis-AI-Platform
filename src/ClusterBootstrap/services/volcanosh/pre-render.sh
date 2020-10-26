#!/bin/bash

dir=`dirname $0`

nsyaml=${dir}/ns.yaml
rbacyaml=${dir}/rbac.yaml

kubectl create -f $nsyaml
kubectl create -f $rbacyaml
