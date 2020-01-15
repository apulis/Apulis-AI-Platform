#!/bin/sh
# 获取宿主机 ip 地址
host_ip=`/sbin/ip route|awk '/default/ { print $3 }'`
# 替换 default.conf proxy_pass ip
sed -i s/"x.x.x.x"/$host_ip/g /etc/nginx/conf.d/default.conf  
# 后台启动 nginx
nginx &
# 启动 ssh 服务
/usr/sbin/sshd -D

