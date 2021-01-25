#! /bin/bash
set -x



# find ib ip, if there is no ib interface, select an available interface
IB_CONFIG_FILE=/job/ib_config-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}
SSH_CONFIG_FILE=/home/${DLWS_USER_NAME}/.ssh/config
WORKER_IB_CONFIG_FILE=/job/.ib_config-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}


if [ ! -f $IB_CONFIG_FILE ];then touch $IB_CONFIG_FILE;fi

get_ib_ip(){
  interface_ip=
  # search an interface ip
  if ifconfig |grep ib -A 1|grep inet ; # if there is ib interface, which is extremely fast, select it
  then
    interface_ip=`ifconfig |grep ib -A 1|grep inet |awk '{print $2}'`
  else # if there is no ib interface, search a available interface
    virtual_interface_array=()
    available_interface=
    for virtual_interface in `ls /sys/devices/virtual/net/`
    do
	  # can't use ${ #array_name } to acquire arraya length due to jinjia transfer syntax
	  num=`echo virtual_interface_array | wc -w`
      virtual_interface_array[${num}]=$virtual_interface
    done
    for network_interface in  `ip addr | grep -v lo| sed -r -n ' s/^[0-9]+: (.*):.*/\1/p'`
    do
      if [[ ! `ifconfig $network_interface | grep "inet " ` ]] ;
	  then
        continue
      fi
      if [[ "${virtual_interface_array[@]}" =~ "$network_interface" ]] ; then
              continue
      else
        available_interface=$network_interface
        break
      fi
    done
    interface_ip=`ifconfig |grep $available_interface -A 1|grep inet |awk '{print $2}'`
  fi
}

if [ "$DLWS_ROLE_NAME" = "worker" ] && command -v ifconfig ;
then
  get_ib_ip
  # check if interface ip is present
  if [ -n "$interface_ip" ]; then


    if ! cat $IB_CONFIG_FILE |grep ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX};
    then
      echo "ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} slots=${DLWS_NUM_GPU_PER_WORKER}" >> $IB_CONFIG_FILE
    else
      sed "s/#ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}.*/'ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} slots=${DLWS_NUM_GPU_PER_WORKER}'/g" -i $IB_CONFIG_FILE
    fi

    # TODO add ib ip to ~/.ssh/config to do "ssh ib-worker-x" without password
    port_key=DLWS_SD_${DLWS_ROLE_NAME}${DLWS_ROLE_IDX}_SSH_PORT
    port=$(printenv $port_key)

    ENV_FILE=/pod.env
    ENVIRONMENT_FILE=/job/.env-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}
    printf "export DLWS_SD_${DLWS_ROLE_NAME}${DLWS_ROLE_IDX}_IB_IP=${interface_ip}\n" >> "${ENVIRONMENT_FILE}";

cat >>${WORKER_IB_CONFIG_FILE} <<EOF

Host ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}
  HostName ${interface_ip}
  Port ${port}
  User ${DLWS_USER_NAME}
  StrictHostKeyChecking no
  UserKnownHostsFile /dev/null

EOF

  fi
fi

HOST_CONFIG_FILE=/job/.hosts
if [ "$DLWS_ROLE_NAME" = "ps" ];then
  files=/job/.ib_config-worker-*
  files_list=${files[@]}
  until [ ${#files_list[@]} == ${DLWS_NUM_WORKER} ]; do
      echo "waiting for all worker write ib ip done"
      sleep 1
      files=/job/.ib_config-worker-*
      files_list=${files[@]}
  done

  if [ ! -f $HOST_CONFIG_FILE ];then touch $HOST_CONFIG_FILE;fi
  cat $HOST_CONFIG_FILE >> /etc/hosts

  for i in /job/.ib_config-worker-*;do
    cat $i >> $SSH_CONFIG_FILE
  done

  for i in /job/.env-worker-*;do
    cat $i >> /job/.env
  done

  echo 1 > /job/ib_ready
fi

if [ "$DLWS_ROLE_NAME" = "worker" ];then
  until [ -f /job/ib_ready ]; do
      echo "waiting for ps0 sync ib message"
      sleep 1
  done
fi

ENV_FILE=/pod.env
sed -i "/_IB_IP/d" ${ENV_FILE}

