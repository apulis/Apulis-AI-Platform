#!/bin/bash
kubectl create serviceaccount job
kubectl create role configmap-admin --verb=get,list,delete --resource=configmaps
kubectl create rolebinding job-configmaps-admin --role=configmap-admin  --serviceaccount=default:job
