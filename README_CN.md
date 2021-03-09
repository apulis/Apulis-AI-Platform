[![build](https://img.shields.io/badge/Build-success-brightgreen.svg)](https://gitee.com/apulisplatform/apulis_platform/releases)
[![license](https://img.shields.io/badge/License-MIT-brightgreen.svg)](LICENSE)
[![release](https://img.shields.io/badge/Release-1.5.0-blue.svg)](https://gitee.com/apulisplatform/apulis_platform/releases/Latest)
[![docs](https://img.shields.io/badge/doc-passing-blueviolet.svg)](https://gitee.com/apulisplatform/apulis_platform/tree/v1.5.0/docs/tutorial)
[![python](https://img.shields.io/badge/python-3.7.5-blue.svg)](https://www.python.org/dev/peps/pep-0537/#id4)

![Apulis标志](docs/img/apulis-logo.png "Apulis logo")

[English](./README.md)|[简体中文](#简介)

<!-- TOC -->

- [简介](#简介)
- [安装指导](#安装指导)
    - [Ansible方式安装](#pip方式安装)
    - [源码编译方式安装](#源码编译方式安装)
    - [Docker镜像](#docker镜像)
- [快速入门](#快速入门)
- [文档](#文档)
- [社区](#社区)
    - [治理](#治理)
    - [交流](#交流)
- [贡献](#贡献)
- [分支维护策略](#分支维护策略)
- [现有分支维护状态](#现有分支维护状态)
- [版本说明](#版本说明)
- [许可证](#许可证)

<!-- /TOC -->

### 简介

**依瞳人工智能平台**旨在为不同行业的用户提供基于深度学习的端到端解决方案，使用户可以用最快的速度、最少的时间开始高性能的深度学习工作，从而大幅节省研究成本、提高研发效率，同时可为中小企业解决私有云难建成、成本高等问题。
平台融合了Tensorflow、PyTorch、MindSpore等开源深度学习框架，提供了模型训练、超参调优、集群状态监控等开发环境，方便AI开发者快速搭建人工智能开发环境，开展AI开发应用。在监控模块基础上搭建预警模块，自动将平台异常通知管理员，提升平台的预警效率及安全性能。

### 安装指导

  * 安装环境须知

  |硬件平台         |操作系统        |状态        |
  |:---------------|:---------------|:----------|
  |Ascend 800-9010 | Ubuntu-18.04.1 |	✔️        |
  |Ascend 800-9000 | Ubuntu-18.04.1 |	✔️        |
  |Atlas 500-3010  | Ubuntu-18.04.1 |	✔️        |
  |GPU CUDA 10.1   | Ubuntu-18.04   |	✔️        |
  |GPU CUDA 11     | Ubuntu-18.04   |	✔️        |
  |CPU             | Ubuntu-18.04   |	✔️        |

  + 集群节点须在同一个局域网（LAN）中
  + 预先开通必要的网络访问端口
  + 建议将etcd，数据库，存储服务独立业务集群

  支持使用ansible交互式分层部署平台，也支持源码配置编译安装。

  - [Ansible方式安装](docs/deployment/atlas_cluster_heterogeneous.md)
  - [源码编译方式安装](docs/deployment/atlas_cluster_heterogeneous.md)
  - [Docker镜像](docs/deployment/docker_images_pull_links.md)

### 快速入门

   请查看[依瞳人工智能平台快速使用指导](docs/)

### 文档

有关安装指南、教程和API的更多详细信息，请参阅[apulis wiki](https://gitee.com/apulis/apulis_platform/wikis/Home)

### 社区

#### 治理

请查看[依瞳人工智能平台开放治理](https://gitee.com/mindspore/community/blob/master/governance.md)。

#### 交流

- [Apulis Slack]() 开发者交流平台。
- `#Apulis`IRC频道（仅用于会议记录）
- 视频会议：待定
- 官网：[依瞳科技](http://www.apulis.cn)
- 邮件列表：[Mailing List](docs/mailing_list.md)
- 微信交流群：
- 微信公众号
- QQ交流群：
- 微博：
- 知乎：


### 贡献

欢迎参与贡献。更多详情，请参阅我们的[贡献者Wiki](docs/CONTRIBUTING.md)。

### 分支维护策略

依瞳人工智能平台的版本分支有以下几种维护阶段：

| **状态**       | **持续时间**    | **说明**                                          |
|-------------|---------------|--------------------------------------------------|
| Planning    | 1 - 3 months  | 特性规划。                     |
| Development | 3 months      | 特性开发。                  |
| Maintained  | 6 - 12 months | 允许所有问题修复的合入，并发布版本。 |
| Unmaintained| 0 - 3 months  | 允许所有问题修复的合入，无专人维护，不再发布版本。                                                 |
| End Of Life (EOL) |  N/A |  不再接受修改合入该分支。    |

### 现有分支维护状态

| **分支名**   | **当前状态**  | **上线时间**          | **后续状态**                          |**EOL 日期**|
|--------------|--------------|----------------------|---------------------------------------|------------|
| **v1.6**     | Development  | 2020-02-10 estimated | Maintained <br> 2022-03-31 estimated  |            |
| **v1.5**     | Maintained   | 2020-12-31           | Unmaintained <br> 2021-06-30 estimated|            |
| **v1.3**     | Unmaintained | 2020-10-30           | End Of Life <br> 2020-12-01 estimated | 2020-11-30 |
| **v0.1.6**   | End Of Life  | 2020-08-31           |                                       | 2020-09-30 |
| **v0.1.0**   | End Of Life  | 2020-07-01           |                                       | 2020-09-30 |


### 版本说明

**Latest Release**
  
+ 更新预置模型和数据集
+ 修复多机分布式任务相关问题
+ 升级UI，更好的使用体验
+ 提升平台稳定性
+ 支持Privilege Job
+ 支持PVC
+ 支持用户资源限制
+ 灵活调度NPU，GPU资源
+ 修复Bug,平台优化
+ 优化UI

版本说明请参阅[RELEASE](docs/RELEASE.md)。

### 许可证

[MIT](LICENSE)
