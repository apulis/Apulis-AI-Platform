#!/bin/bash

dir=`dirname $0`

nsyaml=${dir}/ns.yaml

kubectl create -f $nsyaml
