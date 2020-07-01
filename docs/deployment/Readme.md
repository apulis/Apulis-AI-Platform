# Deployment of AI platform cluster.

The AI platform is designed to provide an end-to-end AI solution to users from different industries and enable them to start the high-performance AI development work with significantly reduced ramp up time, thereby saving costs and improving efficiency. It will also provide a highly efficient, low cost private cloud AI solution for small and medium size company. 

The platform incorporates TensorFlow, PyTorch, MindSpore and other open source AI frameworks, thereby provides user friendly development environment for AI model training, auto ML, hardware status monitoring etc., making it very easy for AI developers to quickly develop AI application. It also has built-in comprehensive early warning system which can automatically alert the system administrator on any anomaly, thereby improve the platform efficiency and security. 

The platform adopts the lightweight virtualization technologies, such as Docker containers that containerizes one or more programs, and provide a standard management interface. Each container is separated from each other. Kubernetes clustering technology is used to orchestrate the containerized applications for planning, automated deployment, updates, and maintenance. 

# Development environment.

Please setup the dev environment of DL workspace as [this](../DevEnvironment/Readme.md). 

# Detailed Step-by-step setup insturction for a selected set of clusters. 

DL workspace cluster can be deployed to either public cloud (e.g., Azure), or to on-prem cluster. The deployment to public cloud is more straightforward, as the environment is more uniform. The deployment instruction are as follows:

## [Azure Container Service](ACS/Readme.md)
## [Azure Cluster](Azure/Readme.md)
## [Azure Deployment using Azure Resource Management (ARM) templates](../../src/ARM/README.md)

We give instruction on the deployment of DL Workspace to an on-prem cluster as well. Please note that because each on-prem cluster is different in hardware (and maybe software) configuration, the deployment procedure is more tricky. The basic deployment step is as follows. 

## [On-Prem, Ubuntu](On-Prem/Ubuntu.md)
## [On-Prem, CoreOS](On-Prem/CoreOS.md)
## [On-Prem, Ubuntu, Single Node](On-Prem/SingleUbuntu.md)

Additional information on general deployment can be found at [here](On-Prem/General.md).






