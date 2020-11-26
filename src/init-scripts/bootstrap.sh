#! /bin/bash
set -ex

# for huawei npu traing jobs only
function generate_envs() {
	ENV_FILE="/pod.env"
	#GCCVERSION=$(gcc --version | grep ^gcc | sed 's/^.* //g')
	ostype=$(arch)
	osflag="arm64"

	if [ "${ostype}" = "aarch64" ]; then
	    osflag="arm64"
	else
	    osflag="x86-64"
	fi

	cat >>${ENV_FILE} <<EOF
export PYTHONPATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe:$PYTHONPATH
export LD_LIBRARY_PATH=/usr/lib/aarch64-linux-gnu/hdf5/serial:/usr/local/Ascend/add-ons:/home/HwHiAiUser/Ascend/nnae/latest/fwkacllib/lib64:/usr/local/Ascend/driver/lib64/common/:/usr/local/Ascend/driver/lib64/driver/:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/arm64-linux/atc/lib64:$LD_LIBRARY_PATH
export TBE_IMPL_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp/op_impl/built-in/ai_core/tbe
export PATH=$PATH:/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/fwkacllib/ccec_compiler/bin/
export ASCEND_OPP_PATH=/home/HwHiAiUser/Ascend/ascend-toolkit/latest/${osflag}-linux/opp

export SOC_VERSION=Ascend910

export RANK_SIZE=1
export RANK_ID=${DLWS_JOB_ID}
export POD_NAME=${DLWS_JOB_ID}
EOF

	IFS=',' read -ra ADDR <<< "$VISIBLE_IDS"
	for i in "${ADDR[@]}"; do
		echo "export DEVICE_ID=$i" >> "${ENV_FILE}"
		echo "export DEVICE_INDEX=$i" >> "${ENV_FILE}"
		break
	done

	## also write those envs for root user
	cat >>"/root/.bashrc" <<SCRIPT
if [ -f ${ENV_FILE} ]; then
    . ${ENV_FILE}
fi
SCRIPT
}

## set npu device configs
function setup_npu_config() {

	npu_info_dir=/home/${DLWS_USER_NAME}/.npu/${DLWS_JOB_ID}
	mkdir -p $npu_info_dir

	## npu distributed job - worker
	if [ "$DLWS_ROLE_NAME" = "worker" ] && [ "$DLWS_IS_NPU_JOB" = "true" ];
	then
		## worker pod
		echo "ip=${NPU_IPS}" >> ${npu_info_dir}/npu_${DLWS_ROLE_IDX}.info
		host_ip=`ip route get 1 | sed -n 's/^.*src \([0-9.]*\) .*$/\1/p'`
		echo "host=${host_ip}" >> ${npu_info_dir}/npu_${DLWS_ROLE_IDX}.info

		generate_envs

		useradd ${DLWS_USER_NAME} -g HwHiAiUser -s /bin/bash -m
		python ${SCRIPT_DIR}/setup_huawei.py --command ${DLWS_LAUNCH_CMD} --out ${npu_info_dir} 
		

	## npu distributed job - master
	elif [ "$DLWS_ROLE_NAME" = "ps" ] && [ "$DLWS_IS_NPU_JOB" = "true" ];
	then
		## master pod, generate hccl.json
		python ${SCRIPT_DIR}/setup_npu.py master
		useradd ${DLWS_USER_NAME} -g HwHiAiUser -s /bin/bash -m
		python ${SCRIPT_DIR}/setup_huawei.py --command ${DLWS_LAUNCH_CMD} --out ${npu_info_dir}

	## not distributed job
	elif [ "$DLWS_ROLE_NAME" = "master" ] && [ ! -z "$NPU_IPS" ];
	then
		## worker pod
		echo "ip=${NPU_IPS}" >> ${npu_info_dir}/npu_0.info
		host_ip=`ip route get 1 | sed -n 's/^.*src \([0-9.]*\) .*$/\1/p'`
		echo "host=${host_ip}" >> ${npu_info_dir}/npu_0.info

		python ${SCRIPT_DIR}/setup_npu.py master
		generate_envs	
		useradd ${DLWS_USER_NAME} -g HwHiAiUser -s /bin/bash -m
		python ${SCRIPT_DIR}/setup_huawei.py --command ${DLWS_LAUNCH_CMD} --out ${npu_info_dir}
	fi

	## create npu log collection script 
	cat > /home/getnpu.sh << EOF
#/bin/bash

mapfile -t device_list < <( ls /dev/ | grep davinci[0-9] )     
device_id_list=()

for device in \${device_list[@]}
do
        id=\${device/davinci/}
        device_id_list+=(\$id)
done

file_list=""
for id in \${device_id_list[@]}
do
        latest_file=\`ls -t /var/log/npu/slog/device-\$id/ | head -n 1\`
        if [ ! -z \$latest_file ]; then
                tail -n 2000 /var/log/npu/slog/device-\$id/\${latest_file} | grep -i ERROR
        fi
done

latest_file=\`ls -t /var/log/npu/slog/host-0/ | head -n 1\`
if [ ! -z \$latest_file ]; then
    tail -n 4000 /var/log/npu/slog/host-0/\${latest_file} | grep -i ERROR
fi 
EOF
	chmod 777 /home/getnpu.sh
}

RUN_TIME_DIR=/dlts-runtime
SCRIPT_DIR=/pod/scripts
LOG_DIR=/pod/logs

mkdir -p ${RUN_TIME_DIR}
mkdir -p ${SCRIPT_DIR}
mkdir -p ${LOG_DIR}

. ${RUN_TIME_DIR}/env/init.env
sh -x ${RUN_TIME_DIR}/install.sh

# set apt mirrors for foreign sources
# sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub

# example: sed -i 's|https\?://[^/]\+/|http://mirrors.aliyun.com/|' /etc/apt/sources.list
# mirror url must be configed in config.yaml like below:
# apt_mirror_url: http:\/\/mirrors.aliyun.com
# sed -i 's|https\?://[^/]\+/|{apt_mirror_url}/|' /etc/apt/sources.list

# to avoid apt-get update error:
# download.nvidia.cn: connection timeout
rm -rf /etc/apt/sources.list.d/cuda.list /etc/apt/sources.list.d/cuda.list.save /etc/apt/sources.list.d/graphics-drivers-ubuntu-ppa-bionic.list /etc/apt/sources.list.d/nvidia-ml.list /etc/apt/sources.list.d/nvidia-ml.list.save

echo bootstrap starts at `date` &>> ${LOG_DIR}/bootstrap.log

# https://stackoverflow.com/a/26759734/845762
# if ! [ -x "$(command -v sudo)" ] ; then
#     time apt-get update && time apt-get install -y sudo
# fi

# if ! [ -x "$(command -v ifconfig)" ];then
#    time apt-get update && time apt-get install -y net-tools
# fi

# if ! [ -x "$(command -v ip)" ];then
#    time 
#     && time apt-get install -y iproute2
# fi

if [ "$DLWS_ROLE_NAME" != "inferenceworker" ];
then
	# Dir for saving running status
	export PROC_DIR=/pod/running
	rm -rf ${PROC_DIR}
	mkdir -p ${PROC_DIR}

	# Dir for logs
	export LOG_DIR=/pod/logs
	#rm -rf ${LOG_DIR}
	mkdir -p ${LOG_DIR}

	# Save the pid.
	PID_FILE=${PROC_DIR}/pid
	echo $$ > $PID_FILE
fi

# Setup container
echo "==========================begin to create user and setup env =============" &>> ${LOG_DIR}/bootstrap.log
if id "${DLWS_USER_NAME}" &>/dev/null;
then
    echo "User ${DLWS_USER_NAME} found, skip init_user.sh ..."
else
    bash ${SCRIPT_DIR}/init_user.sh &>> ${LOG_DIR}/bootstrap.log
fi
echo "==========================create user done!===============================" &>> ${LOG_DIR}/bootstrap.log

if [ "$DLWS_ROLE_NAME" != "inferenceworker" ];
then
	touch ${PROC_DIR}/CONTAINER_READY
fi

# setup ib config
echo "===========================begin to setup ib config=============================="&>> ${LOG_DIR}/bootstrap.log
bash ${SCRIPT_DIR}/setup_ib_config.sh &>> ${LOG_DIR}/bootstrap.log
echo "===========================setup ib config done!================================="&>> ${LOG_DIR}/bootstrap.log

# Setup roles
echo "===========================begin to start ssh=============================="&>> ${LOG_DIR}/bootstrap.log
bash ${SCRIPT_DIR}/setup_sshd.sh &>> ${LOG_DIR}/bootstrap.log
echo "===========================start ssh done!================================="&>> ${LOG_DIR}/bootstrap.log

if [ "$DLWS_ROLE_NAME" != "inferenceworker" ];
then
  echo "=========================begin to setup ssh!============================="&>> ${LOG_DIR}/bootstrap.log
    bash ${SCRIPT_DIR}/setup_ssh_config.sh &>> ${LOG_DIR}/bootstrap.log
	touch ${PROC_DIR}/ROLE_READY
	echo "=========================setup ssh done!================================"&>> ${LOG_DIR}/bootstrap.log

	# Setup job
	# TODO
	touch ${PROC_DIR}/JOB_READY
fi

# create path for training jobs
echo "=========================begin to setup path!============================="&>> ${LOG_DIR}/bootstrap.log
if [ ! -z ${CODE_PATH} ]; then
	runuser -l ${DLWS_USER_NAME} -c "mkdir -p ${CODE_PATH}"
fi

if [ ! -z ${OUTPUT_PATH} ]; then
	runuser -l ${DLWS_USER_NAME} -c "mkdir -p ${OUTPUT_PATH}"
fi

# setup npu device info for npu distributing jobs
npu_info_dir=/home/${DLWS_USER_NAME}/.npu/${DLWS_JOB_ID}
runuser -l ${DLWS_USER_NAME} -c "mkdir -p ${npu_info_dir}"
echo "=========================setup path done!============================="&>> ${LOG_DIR}/bootstrap.log


echo "===========================begin to setup npu config=============================="&>> ${LOG_DIR}/bootstrap.log
setup_npu_config
echo "===========================setup npu config done!=============================="&>> ${LOG_DIR}/bootstrap.log


echo bootstrap ends at `date` &>> ${LOG_DIR}/bootstrap.log
set +e

# Execute user's command for the job
if ([ "$DLWS_ROLE_NAME" = "worker" ] && [ "$DLWS_IS_NPU_JOB" = "false" ]) || ([ "$DLWS_ROLE_NAME" = "ps" ] && [ "$DLWS_IS_NPU_JOB" = "true" ]);
then
    runuser -l ${DLWS_USER_NAME} -c "sleep infinity"
else
#    if ([ "$DLWS_ROLE_NAME" = "worker" ] && [ "$DLWS_IS_NPU_JOB" = "true" ]);
#    then
#      DLWS_LAUNCH_CMD="${DLWS_LAUNCH_CMD}  && sleep infinity"
#    fi
    printenv DLWS_LAUNCH_CMD > /pod/job_command.sh
    chmod ugo+rx /pod/job_command.sh
    chmod ugo+rx /pod.env
    echo "============================begin to exec command!=========================="&>> ${LOG_DIR}/bootstrap.log
    runuser -l ${DLWS_USER_NAME} -c /pod/job_command.sh
    # Save exit code
    EXIT_CODE=$?
    echo  `date` ": ${EXIT_CODE}"  > ${PROC_DIR}/EXIT_CODE
fi

# exit
exit ${EXIT_CODE}
