#!/bin/bash
kubectl create serviceaccount job
kubectl create role configmap-admin --verb=get,list,delete,create --resource=configmap
kubectl create rolebinding job-configmap-admin --role=configmap-admin  --serviceaccount=default:job
