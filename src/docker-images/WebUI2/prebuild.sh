#!/bin/bash 
# This command will be executed under directory 
# src/ClusterBootstrap/deploy/docker-images/.../

rm -rf WebPortal
cd ../../../
python deploy.py webui
cd deploy/docker-images/WebUI2
cp -r ../../../../WebUI2/dotnet/WebPortal WebPortal
