#bash
if [[ $DLWS_SD_SELF_IP == '192.168.3.2' ]];
then
	./run_distribute_train_2.sh /data/resnet50_cifar10/rank_table_16pcs.json /data/resnet50_cifar10/cifar
else
	./run_distribute_train_1.sh /data/resnet50_cifar10/rank_table_16pcs.json /data/resnet50_cifar10/cifar
fi
