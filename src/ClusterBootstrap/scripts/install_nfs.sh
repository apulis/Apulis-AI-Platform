#sudo apt-get update
#sudo apt-get install nfs-kernel-server nfs-common portmap
sudo ln -s /etc/init.d/nfs-kernel-server /etc/init.d/nfs

sudo /etc/init.d/nfs-kernel-server restart

sudo echo "/mnt/local               *(rw,fsid=0,insecure,no_subtree_check,async,no_root_squash)" | sudo tee -a /etc/exports
sudo exportfs -a
