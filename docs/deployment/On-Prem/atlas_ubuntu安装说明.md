## 系统安装

### 安装ubuntu步骤

- 官方说明

  > https://support.huawei.com/enterprise/en/doc/EDOC1100048786/916c5c0a/mounting-a-file-to-the-virtual-cd-rom-drive-vmm--d-connect

- ibmc登录

  > ssh Administrator@192.168.1.21
  >
  > 密码和用户名，与ibmc同
  >
  > 
  >
  > 192.168.1.21为ibmc地址
  >
  > 查询ibmc地址：ipmctool lan print 1

- 挂载iso镜像

  > 查询挂载情况：ipmcget -t vmm -d info
  >
  > 执行挂载：ipmcset -t vmm -d connect -v nfs://192.168.1.3/data/nfsshare/iso/ubuntu-18.04.1-server-arm64.iso
  >
  > 执行卸载：ipmcset -t vmm -d disconnect

- 配置系统启动顺序

  > 配置为光驱启动
  >
  > 路径：ibmc主页  -->系统管理 --> BIOS配置
  >
  > 配置：光驱启动，保存配置

- IBMC配置

  > 1. 安装JAVA RUNTIME Environment
  > 2. 将目标机器的IBMC 2198和8208端口转发打开（ibmc只开放同样端口），把其它机器的IBMC端口转发规则禁用
  
- ubuntu系统重装

  > 1. ibmc主页  -->虚拟控制台 --> JAVA集成远程控制台
  > 2. 登陆OS，执行reboot后重装ubuntu
  
- 重置系统加载顺序
  
  > 将启动顺序 重新配置为 硬盘加载
  
  
  

### 安装NPU驱动

- 拷贝ceph配置文件

  > 1. mkdir -p /etc/ceph
  >
  > 2. scp root@192.168.3.2:/home/bifeng.peng/ceph-config/atlas01.tar . 
  >
  > 3. 将atlas01.tar文件解压至/etc/ceph
  >
  > 4. 挂载ceph
  >
  >    ceph-fuse -m 192.168.2.16 /mnt/fuse

- 安装配置

  > 安装此脚本内的所有安装函数
  >
  > /mnt/fuse/atlas01-pbf/install-atlas/install.sh 

- 配置NPU设备IP -- 3.6 -- atlas01

  > hccn_tool -i 0 -ip -s address 192.168.10.1 netmask 255.255.255.0
  > hccn_tool -i 1 -ip -s address 192.168.20.1 netmask 255.255.255.0
  > hccn_tool -i 2 -ip -s address 192.168.30.1 netmask 255.255.255.0
  > hccn_tool -i 3 -ip -s address 192.168.40.1 netmask 255.255.255.0
  > hccn_tool -i 4 -ip -s address 192.168.10.2 netmask 255.255.255.0
  > hccn_tool -i 5 -ip -s address 192.168.20.2 netmask 255.255.255.0
  > hccn_tool -i 6 -ip -s address 192.168.30.2 netmask 255.255.255.0
  > hccn_tool -i 7 -ip -s address 192.168.40.2 netmask 255.255.255.0

- 检查网口配置状态是否正确 (状态UP为正确)

  > for i in {0..7}; do hccn_tool -i ${i} -link -g; done

- 导入镜像

  > cd /mnt/fuse/atlas01-image/
  >
  > ./image.sh load

- 检查NPU驱动状态

  之前我是通过搭建虚拟环境, 再通过官网的mindspore demo来验证NPU驱动是否安装正确;

  此处似可以通过mindspore-withtools直接验证



### 安装网卡驱动

- 下载以下说明文档

  > [《华为 TM210 灵活IO卡 用户指南 01.PDF》](https://apulistech-my.sharepoint.com/personal/kelly_zhang_apulis_com/_layouts/15/onedrive.aspx?id=%2Fpersonal%2Fkelly%5Fzhang%5Fapulis%5Fcom%2FDocuments%2FApulis%20File%20Share%2FPM%E9%83%A8%2FDavinci%E9%A1%B9%E7%9B%AE%2F05%20%E5%8D%8E%E4%B8%BA%E6%8F%90%E4%BE%9B%E7%9A%84%E8%B5%84%E6%96%99%2FA910%E5%B8%AE%E5%8A%A9%E6%96%87%E6%A1%A30626%2F%E5%8D%8E%E4%B8%BA%20TM210%20%E7%81%B5%E6%B4%BBIO%E5%8D%A1%20%E7%94%A8%E6%88%B7%E6%8C%87%E5%8D%97%2003%2Epdf&parent=%2Fpersonal%2Fkelly%5Fzhang%5Fapulis%5Fcom%2FDocuments%2FApulis%20File%20Share%2FPM%E9%83%A8%2FDavinci%E9%A1%B9%E7%9B%AE%2F05%20%E5%8D%8E%E4%B8%BA%E6%8F%90%E4%BE%9B%E7%9A%84%E8%B5%84%E6%96%99%2FA910%E5%B8%AE%E5%8A%A9%E6%96%87%E6%A1%A30626)

- 安装驱动：章节4.2.3



### 配置自动挂载

- 创建开机启动脚本

  vim /etc/rc.local

```shell
#!/bin/bash
dhclient
ceph-fuse -m 192.168.2.16 /mnt/fuse
exit 0
```

- 设置开机启动

  ```
  sudo chmod 755 /etc/rc.local
  sudo systemctl enable rc-local
  sudo systemctl restart rc-local
  sudo systemctl status rc-local
  ```

- 重启server检验IP配置