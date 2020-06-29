#! /bin/bash
set -x

# generate ps host list
ps_host_list=""
for i in $(seq 0 $(( ${DLWS_NUM_PS} - 1 )) )
do
    ps_host_list+="ps-${i} "
done

# generate worker host list
worker_host_list=""
if [ "$DLWS_ROLE_NAME" = "master" ];
then
    worker_host_list="${DLWS_ROLE_NAME}"
else
    for i in $(seq 0 $(( ${DLWS_NUM_WORKER} - 1 )) )
    do
        worker_host_list+="worker-${i} "
    done
fi

# generate host list
host_list="${ps_host_list} ${worker_host_list}"

# generate ~/.ssh/config
SSH_CONFIG_FILE=/home/${DLWS_USER_NAME}/.ssh/config
NPU_CONFIG_FILE=/home/${DLWS_USER_NAME}/.ssh/npu_config

>${SSH_CONFIG_FILE}
>${NPU_CONFIG_FILE}

chown ${DLWS_USER_NAME} ${SSH_CONFIG_FILE}
chmod 600 ${SSH_CONFIG_FILE}

for host in ${host_list}
do
    if [ "$DLWS_ROLE_NAME" = "master" ];
    then
        ip=$DLWS_SD_SELF_IP
        port=$DLWS_SD_SELF_SSH_PORT
        host_ip=$DLWS_SD_SELF_HOST_IP
    else
        role=${host%%-*}
        idx=${host##*-}

        ip_key=DLWS_SD_${role}${idx}_IP
        ib_ip_key=DLWS_SD_${role}${idx}_IB_IP
        port_key=DLWS_SD_${role}${idx}_SSH_PORT

        npu_ip_list_key=DLWS_SD_${role}${idx}_SSH_PORT
        host_ip_key=DLWS_SD_${idx}_HOST_IP

        ip=$(printenv $ip_key)
        ib_ip=$(printenv $ib_ip_key)
        port=$(printenv $port_key)

        npu_ip_list=$(printenv $npu_ip_list_key)
        host_ip=$(printenv $host_ip_key)
    fi
    cat >>${SSH_CONFIG_FILE} <<EOF

Host ${host}
  HostName ${ip}
  Port ${port}
  User ${DLWS_USER_NAME}
  StrictHostKeyChecking no
  UserKnownHostsFile /dev/null

EOF

    # also add entry to /etc/hosts
    echo -e "${ip}\t${host}" >> /etc/hosts
done

# generate npu info for distributed npu jobs
if [ ! -z npu_ip_list ] && [ "$role" = "worker" ]; then
    cat >> ${NPU_CONFIG_FILE} << EOF
${npu_ip_list} slots=${DLWS_NUM_GPU_PER_WORKER}
${host_ip} slots=${DLWS_NUM_GPU_PER_WORKER}
EOF
fi

envs=(
LD_LIBRARY_PATH
LIBRARY_PATH
PATH
PYTHONPATH
NCCL_IB_DISABLE
NCCL_VERSION
DLWS_HOST_NETWORK
DLWS_JOB_ID
DLTS_JOB_TOKEN
DLWS_NUM_PS
DLWS_NUM_WORKER
DLWS_NUM_GPU_PER_WORKER
DLWS_NUM_WORKER
DLWS_VC_NAME
DLWS_UID
DLWS_GID
DLWS_USER_NAME
DLWS_USER_EMAIL
DLWS_ROLE_NAME
DLWS_ROLE_IDX
)

SSH_ENVIRONMENT_FILE=/home/${DLWS_USER_NAME}/.ssh/environment
for env_key in "${envs[@]}" ; do
    if [ "`printenv $env_key`" != "" ] ; then
        printf $env_key >> $SSH_ENVIRONMENT_FILE
        printf = >> $SSH_ENVIRONMENT_FILE
        printenv $env_key >> $SSH_ENVIRONMENT_FILE
    fi
done
chown ${DLWS_USER_NAME} ${SSH_ENVIRONMENT_FILE}
chmod 600 ${SSH_ENVIRONMENT_FILE}

mkdir -p /root/.ssh && cp /home/${DLWS_USER_NAME}/.ssh/* /root/.ssh/ && chown root /root/.ssh/* && chmod 600 /root/.ssh/*

# generate /job/hostfile
if [ "$DLWS_ROLE_NAME" = "master" ] || [ "$DLWS_ROLE_NAME" = "ps" ];
then
    SLOT_FILE="/job/hostfile"
    >${SLOT_FILE}
    chown ${DLWS_USER_NAME} ${SLOT_FILE}

    for host in ${worker_host_list}
    do
        slots=${DLWS_NUM_GPU_PER_WORKER}
        cat >>${SLOT_FILE} <<EOF
${host} slots=${slots}
EOF
    done
fi

# make sure worker have sshd up and running
if [ "$DLWS_ROLE_NAME" = "ps" ];
then
    for host in ${host_list}
    do
        succ=false
        for i in `seq 1 3600` ; do
            echo "testing $host"
            ssh $host "echo 1"
            # do not add code here
            rtn=$?
            echo "done testing $host"
            if [ "$rtn" -eq "0" ] ; then
                succ=true
                echo "$host has done sshd setup"
                break
            else
                echo "$host has not done sshd setup wait 1s"
                sleep 1
            fi
        done
        if [ "$succ" = "false" ] ; then
            exit 1
        fi
    done
fi