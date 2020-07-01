#!/bin/bash
/opt/resolve_ip.sh
ETCDIP=$(cat /opt/defaultip)
/usr/bin/docker rm -f etcd3 
/usr/bin/docker run -v /usr/share/ca-certificates/mozilla:/etc/ssl/certs -v /etc/etcd/ssl:/etc/etcd/ssl -v /var/etcd:/var/etcd -p 2379:2379 -p 2380:2380 \
  --net=host \
  --name etcd3 dlws/etcd:3.1.10 /usr/local/bin/etcd \
  -name $HOSTNAME \
  -initial-cluster $HOSTNAME=https://$ETCDIP:2380 \
  -initial-cluster-state new \
  -initial-cluster-token 3924e5ca-00c9-4245-a49f-08f817d1d954 \
  -advertise-client-urls https://$ETCDIP:2379 \
  -listen-client-urls https://0.0.0.0:2379 \
  -initial-advertise-peer-urls https://$ETCDIP:2380 \
  -listen-peer-urls https://0.0.0.0:2380 \
  -data-dir /var/etcd/data \
  -client-cert-auth \
  -trusted-ca-file=/etc/etcd/ssl/ca.pem \
  -cert-file=/etc/etcd/ssl/etcd.pem \
  -key-file=/etc/etcd/ssl/etcd-key.pem \
  -peer-client-cert-auth \
  -peer-trusted-ca-file=/etc/etcd/ssl/ca.pem \
  -peer-cert-file=/etc/etcd/ssl/etcd.pem \
  -peer-key-file=/etc/etcd/ssl/etcd-key.pem