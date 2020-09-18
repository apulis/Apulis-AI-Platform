#!/bin/bash 
# This command will be executed under directory 
# src/ClusterBootstrap/deploy/docker-images/.../

rm -rf DLDocs
cp -r ../../../../DLDocs .
cp -r ../../../../DLDocs/Dockerfile .