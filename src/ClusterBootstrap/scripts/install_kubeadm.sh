sudo apt-get update && sudo apt-get install -y apt-transport-https curl
sudo curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | sudo apt-key add -

cat <<EOF | sudo tee /etc/apt/sources.list.d/kubernetes.list
deb https://mirrors.aliyun.com/kubernetes/apt/ kubernetes-xenial main
EOF

k8s_version="1.18.0-00"

sudo apt-get update
sudo apt-get install -y kubelet=${k8s_version} kubeadm=${k8s_version} kubectl=${k8s_version}
sudo apt-mark hold kubelet kubeadm kubectl
