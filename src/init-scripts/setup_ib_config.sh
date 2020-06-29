#! /bin/bash
set -x


# find ib ip
HOST_CONFIG_FILE=/job/.hosts
IB_CONFIG_FILE=/job/ib_config
if [ "$DLWS_ROLE_NAME" = "worker" ] && command -v ifconfig ;
then
  ib_ip=`ifconfig |grep ib -A 1|grep inet |awk '{print $2}'`
  if ifconfig |grep ib -A 1|grep inet ;
  then
    if [ ! -f $IB_CONFIG_FILE ];then touch $IB_CONFIG_FILE;fi
    if [ ! -f $HOST_CONFIG_FILE ];then touch $HOST_CONFIG_FILE;fi
    if ! cat $IB_CONFIG_FILE |grep ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX};
    then
      echo "ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} slots=${DLWS_NUM_GPU_PER_WORKER}" >> $IB_CONFIG_FILE
    else
      sed "s/#ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}.*/'ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} slots=${DLWS_NUM_GPU_PER_WORKER}'/g" -i $IB_CONFIG_FILE
    fi
    if ! cat $HOST_CONFIG_FILE |grep ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} ;
    then
      echo -e "${ib_ip}\tib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}" >> $HOST_CONFIG_FILE
    else
      sed "s/.*ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}/${ib_ip}\tib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}/" -i $HOST_CONFIG_FILE
    fi
  fi
fi

if [ "$DLWS_ROLE_NAME" = "ps" ];then
  if [ ! -f $HOST_CONFIG_FILE ];then touch $HOST_CONFIG_FILE;fi
  cat $HOST_CONFIG_FILE >> /etc/hosts
fi