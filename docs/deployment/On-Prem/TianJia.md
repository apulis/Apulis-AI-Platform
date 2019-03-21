# 在钱江源平台中添加GPU服务器

步骤如下

1. 使用Ubuntu16.04系统，初始化GPU服务器，设置本地存储
（建议系统盘用RAID-1的1TB SSD硬盘，其他硬盘用RAID-5后安装在/mnt/local)
2. 在目标服务器上运行./deploy/etc/inituser.sh
(目的：创建GPU服务器运转账号，初始化sshkey登陆，使运转账号可以无密码运转sudo/docker)
3. 设置GPU服务器主机名
```
  sudo hostnamectl set-hostname <name>
```
将该GPU服务器主机加入DNS服务器，使其可以被使用。
4. 在GPU服务器上安装NVidia驱动和NVidia-docker2，安装软件包。
```
  ./deploy.py --node <name> runscriptonall ./script/prepare_ubuntu_cn.sh
```
5. 将服务器加入 config.yaml 表中。
6. 安装共享存储
```
  ./deploy.py --node <name> mount
```
6. 安装kubelet, 注册
```
  ./deploy.py --node <name> --verbose -y updateworker
  ./deploy.py --node <name> kubernetes labels
```
