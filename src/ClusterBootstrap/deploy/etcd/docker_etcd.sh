#export HostIP=$(ip route get 8.8.8.8 | awk '{print $NF; exit}') && \
#curl -w "\n" 'https://discovery.etcd.io/new?size=3'

docker run -d -v /usr/share/ca-certificates/:/etc/ssl/certs -v /var/etcd:/var/etcd -p 2379:2379 -p 2380:2380 \
 --net=host \
 --restart always \
 --name etcd dlws/etcd:3.1.10 /usr/local/bin/etcd \
 -name $HOSTNAME \
 -initial-cluster =https://:2380 \
 -initial-cluster-state new \
 -initial-cluster-token 3924e5ca-00c9-4245-a49f-08f817d1d954 \
 -advertise-client-urls https://:2379 \
 -listen-client-urls https://0.0.0.0:2379 \
 -initial-advertise-peer-urls https://:2380 \
 -listen-peer-urls https://0.0.0.0:2380 \
 -data-dir /var/etcd/data \