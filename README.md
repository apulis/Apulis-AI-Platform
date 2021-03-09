[![build](https://img.shields.io/badge/Build-success-brightgreen.svg)](https://gitee.com/apulisplatform/apulis_platform/releases)
[![license](https://img.shields.io/badge/License-MIT-brightgreen.svg)](LICENSE)
[![release](https://img.shields.io/badge/Release-1.5.0-blue.svg)](https://gitee.com/apulisplatform/apulis_platform/releases/Latest)
[![docs](https://img.shields.io/badge/doc-passing-blueviolet.svg)](https://gitee.com/apulisplatform/apulis_platform/tree/v1.5.0/docs/tutorial)
[![python](https://img.shields.io/badge/python-3.7.5-blue.svg)](https://www.python.org/dev/peps/pep-0537/#id4)
[![Gitter](https://badges.gitter.im/apulis-ai-platform/community.svg)](https://gitter.im/apulis-ai-platform/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

![Apulis标志](docs/img/apulis-logo.png "Apulis logo")

[English](#Overview)|[简体中文](./README_CN.md)

## Overview

**Apulis AI Platform** is designed to provide an end-to-end AI solution to users from different industries and enable them to start the high-performance AI development work with significantly reduced ramp up time, thereby saving costs and improving efficiency. It will also provide a highly efficient, low cost private cloud AI solution for small and medium size company. 

The platform incorporates TensorFlow, PyTorch, MindSpore and other open source AI frameworks, thereby provides user friendly development environment for AI model training, auto ML, hardware status monitoring etc., making it very easy for AI developers to quickly develop AI application. It also has built-in comprehensive early warning system which can automatically alert the system administrator on any anomaly, thereby improve the platform efficiency and security. 

The platform adopts the lightweight virtualization technologies, such as Docker containers that containerizes one or more programs, and provide a standard management interface. Each container is separated from each other. Kubernetes clustering technology is used to orchestrate the containerized applications for planning, automated deployment, updates, and maintenance.. 

## Directory Structure
```
|-- devenv                          Dockerfile for creating dev environment on amd64 arch
|-- devenv.arm64                    Dockerfile for creating dev environment on arm64 arch
|-- docs
|   |-- deployment
|   `-- tutorial
|-- example
|   `-- resnet50_cifar10
|-- License
`-- src
    |-- ARM
    |-- ClusterBootstrap            deployment module
    |-- ClusterManager              main backend module
    |-- ClusterPortal
    |-- Jobs_Templete               AI Job template
    |-- RepairManager               Alert module
    |-- RestAPI                     API module
    |-- StorageManager
    |-- WebUI                       Frontend module
    |-- WebUI2
    |-- dashboard
    |-- dev-utils		    
    |-- docker-images               Miscellaneus components          
    |-- init-scripts                Initialization scripts for AI jobs
    |-- user-dashboard
    |-- user-synchronizer
    `-- utils                       utility module
```
