sudo rm -rf lib
sudo mkdir lib
sudo rm npu-smi
sudo cp /lib/ld-linux-aarch64.so.1 lib/
sudo cp /usr/local/Ascend/driver/lib64/common/libslog.so lib/
sudo cp /usr/local/Ascend/driver/lib64/common/libc_sec.so lib/
sudo cp /usr/local/Ascend/driver/lib64/common/libmmpa.so lib/
sudo cp /usr/local/Ascend/driver/lib64/driver/libdrvhdc_host.so lib/
sudo cp /usr/local/Ascend/driver/lib64/driver/libdrvdevdrv.so lib/
sudo cp /usr/local/Ascend/driver/lib64/driver/libdrvdsmi_host.so lib/
sudo cp /usr/local/sbin/npu-smi npu-smi
sudo chmod -R 777 lib/ npu-smi