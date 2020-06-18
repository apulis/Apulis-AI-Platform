#! /bin/bash
set -ex

. /dlts-runtime/env/init.env
sh -x /dlts-runtime/install.sh

# set apt mirrors for foreign sources
#sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub
sed -i 's|https\?://[^/]\+/|http://mirrors.aliyun.com/|' /etc/apt/sources.list 

# to avoid apt-get update error: 
# download.nvidia.cn: connection timeout
rm -rf /etc/apt/sources.list.d/cuda.list /etc/apt/sources.list.d/cuda.list.save /etc/apt/sources.list.d/graphics-drivers-ubuntu-ppa-bionic.list /etc/apt/sources.list.d/nvidia-ml.list /etc/apt/sources.list.d/nvidia-ml.list.save

SCRIPT_DIR=/pod/scripts
echo bootstrap starts at `date` &>> ${LOG_DIR}/bootstrap.log

# https://stackoverflow.com/a/26759734/845762
if ! [ -x "$(command -v sudo)" ] ; then
    time apt-get update && time apt-get install -y sudo net-tools
fi

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
bash ${SCRIPT_DIR}/init_user.sh # &>> ${LOG_DIR}/bootstrap.log

if [ "$DLWS_ROLE_NAME" != "inferenceworker" ];
then
	touch ${PROC_DIR}/CONTAINER_READY
fi

# Setup roles
bash ${SCRIPT_DIR}/setup_sshd.sh &>> ${LOG_DIR}/bootstrap.log

if [ "$DLWS_ROLE_NAME" != "inferenceworker" ];
then
    bash ${SCRIPT_DIR}/setup_ssh_config.sh &>> ${LOG_DIR}/bootstrap.log
	touch ${PROC_DIR}/ROLE_READY

	# Setup job
	# TODO
	touch ${PROC_DIR}/JOB_READY
fi

echo bootstrap ends at `date` &>> ${LOG_DIR}/bootstrap.log

set +e
# Execute user's command for the job
if ([ "$DLWS_ROLE_NAME" = "worker" ] && [ "$DLWS_IS_NPU_JOB" = "false" ]) || ([ "$DLWS_ROLE_NAME" = "ps" ] && [ "$DLWS_IS_NPU_JOB" = "true" ]);
then
    runuser -l ${DLWS_USER_NAME} -c "sleep infinity"
else
    mkdir -p /pod/${DLWS_JOB_ID}
    printenv DLWS_LAUNCH_CMD > /pod/${DLWS_JOB_ID}/job_command.sh
    chmod ugo+rx /pod/${DLWS_JOB_ID}/job_command.sh
    chmod ugo+rx /pod.env
    runuser -l ${DLWS_USER_NAME} -c /pod/${DLWS_JOB_ID}/job_command.sh
    # Save exit code
    EXIT_CODE=$?
    echo  `date` ": ${EXIT_CODE}"  > ${PROC_DIR}/EXIT_CODE
fi

# exit
exit ${EXIT_CODE}
