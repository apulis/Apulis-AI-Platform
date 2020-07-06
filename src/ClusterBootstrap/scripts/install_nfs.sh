sudo apt-get update
sudo apt-get install nfs-kernel-server nfs-common portmap
sudo ln -s /etc/init.d/nfs-kernel-server /etc/init.d/nfs

sudo /etc/init.d/nfs-kernel-server restart
