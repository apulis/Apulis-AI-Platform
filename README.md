# Apulis AI Platform

The AI platform is designed to provide an end-to-end AI solution to users from different industries and enable them to start the high-performance AI development work with significantly reduced ramp up time, thereby saving costs and improving efficiency. It will also provide a highly efficient, low cost private cloud AI solution for small and medium size company. 

The platform incorporates TensorFlow, PyTorch, MindSpore and other open source AI frameworks, thereby provides user friendly development environment for AI model training, auto ML, hardware status monitoring etc., making it very easy for AI developers to quickly develop AI application. It also has built-in comprehensive early warning system which can automatically alert the system administrator on any anomaly, thereby improve the platform efficiency and security. 

The platform adopts the lightweight virtualization technologies, such as Docker containers that containerizes one or more programs, and provide a standard management interface. Each container is separated from each other. Kubernetes clustering technology is used to orchestrate the containerized applications for planning, automated deployment, updates, and maintenance. 

## Directory Structure
```
|-- devenv                          Dockerfile for creating dev environment on amd64 arch
|-- devenv.arm64                    Dockerfile for creating dev environment on arm64 arch
|-- docs
|   |-- KnownIssues
|   |-- Presentation
|   |-- deployment
|   `-- tutorial
|-- example
|   `-- resnet50_cifar10
|-- license.old
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
