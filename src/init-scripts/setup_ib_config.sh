#! /bin/bash
set -x


function prepare_constants() {

    HOST_CONFIG_FILE=/job/.hosts
    IB_CONFIG_FILE=/job/ib_config
    SSH_CONFIG_FILE=/home/${DLWS_USER_NAME}/.ssh/config

    WORKER_IB_CONFIG_FILE=/job/.ib_config

    # ib config file for current pod
    # for single-node job, DLWS_ROLE_IDX=0
    SSH_IB_CONFIG_FILE=/home/${DLWS_USER_NAME}/.ssh/worker_ib_config_${DLWS_ROLE_IDX}
}

# judge if it is worker pod
# distributed job
function is_worker_pod() {

    if [ "$DLWS_ROLE_NAME" = "worker" ]; then

        # true
        return 0
    else

        # false
        return 1
    fi
}

# judge if it is ps pod
# distributed job
function is_ps_pod() {

    if [ "$DLWS_ROLE_NAME" = "ps" ]; then

        # true
        return 0
    else

        # false
        return 1
    fi
}

# judge if it is master pod
# single machine job
function is_master_pod() {

    if [ "$DLWS_ROLE_NAME" = "master" ]; then

        # true
        return 0
    else

        # false
        return 1
    fi
}


# setup ib config for ps pod
# job type: distributed job
function setup_ps_ib_config() {

    # wait for ib configs of worker pod
    timeout=3600 # seconds

   
    for i in `seq 0 $(( ${DLWS_NUM_WORKER} - 1 ))`
    do 
        counter=0
        for j in `seq 1 ${timeout}` ; do

            IB_FILE=/home/${DLWS_USER_NAME}/.ssh/worker_ib_config_${i}
            echo "ps pod. checking ib config ${IB_FILE}"

            if test -f "$IB_FILE"; then
                echo "$IB_FILE has been created "
                break
            else
                echo "$IB_FILE not found. wait 1 second"
                sleep 1
                let "counter+=1" 
            fi

        done 

        if [ $counter -ge ${timeout} ]; 
        then
            echo "ib config file ${IB_FILE} not found. exit"
            exit 1
        fi
        
    done

    # combine ib config of workers into ssh config
    for i in `seq 0 $(( ${DLWS_NUM_WORKER} - 1 ))`
    do
        IB_FILE=/home/${DLWS_USER_NAME}/.ssh/worker_ib_config_${i}
	cat ${IB_FILE} >> ${SSH_CONFIG_FILE}
    done

    echo "ps pod setting ib configs done!"
}


# setup ib config for master pod
# job type: single node job
function setup_master_ib_config() {

    #setup_worker_ib_config
    #setup_ps_ib_config
    :
}


# setup ib config for worker pod
# job type: distributed job
function setup_worker_ib_config() {
    

    # find ib ip, if there is no ib interface, select an available interface
    # a begin
    if command -v ifconfig ;
    then

      interface_ip=
      # search an interface ip

      # b begin
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
          # c begin
          if [[ ! `ifconfig $network_interface | grep "inet " ` ]] ;
    	  then
            continue
          fi  
	  # c ends

	  # d begin
          if [[ "${virtual_interface_array[@]}" =~ "$network_interface" ]] ; then
                  continue
          else
            available_interface=$network_interface
            break 
          fi 
	  # d ends

        done

        interface_ip=`ifconfig |grep $available_interface -A 1|grep inet |awk '{print $2}'`

      fi
      # b ends
    
      # check if interface ip is present
      # e bgin
      if [ -n "$interface_ip" ]; then

        if [ ! -f $IB_CONFIG_FILE ];then touch $IB_CONFIG_FILE;fi
        if [ ! -f $HOST_CONFIG_FILE ];then touch $HOST_CONFIG_FILE;fi
    
        # f begin 
        if ! cat $IB_CONFIG_FILE |grep ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX};
        then
          echo "ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} slots=${DLWS_NUM_GPU_PER_WORKER}" >> $IB_CONFIG_FILE
        else
          sed "s/#ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}.*/'ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} slots=${DLWS_NUM_GPU_PER_WORKER}'/g" -i $IB_CONFIG_FILE
        fi
	# f ends
    
        # TODO add ib ip to ~/.ssh/config to do "ssh ib-worker-x" without password
        port_key=DLWS_SD_${DLWS_ROLE_NAME}${DLWS_ROLE_IDX}_SSH_PORT
        port=$(printenv $port_key)
    
    cat >>${SSH_IB_CONFIG_FILE} <<EOF
    
    Host ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}
      HostName ${interface_ip}
      Port ${port}
      User ${DLWS_USER_NAME}
      StrictHostKeyChecking no
      UserKnownHostsFile /dev/null
    
EOF
    
    if [ ! -f $WORKER_IB_CONFIG_FILE ];then touch $WORKER_IB_CONFIG_FILE;fi
    cat >>${WORKER_IB_CONFIG_FILE} <<EOF
    
    Host ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}
      HostName ${interface_ip}
      Port ${port}
      User ${DLWS_USER_NAME}
      StrictHostKeyChecking no
      UserKnownHostsFile /dev/null
    
EOF
    
        # add entry to /etc/hosts
	# g begin
        if ! cat $HOST_CONFIG_FILE |grep ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX} ;
        then
          echo -e "${interface_ip}\tib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}" >> $HOST_CONFIG_FILE
        else
          sed "s/.*ib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}/${interface_ip}\tib-${DLWS_ROLE_NAME}-${DLWS_ROLE_IDX}/" -i $HOST_CONFIG_FILE
        fi 
	# g ends

    
      fi # e ends
      
    fi # a ends
}

# setup ib config
function setup_ib_config() {

    if is_master_pod ; then
        
        setup_master_ib_config

    elif is_ps_pod ; then
  
        setup_ps_ib_config

    elif is_worker_pod ; then
  
        setup_worker_ib_config

    fi 
}

# combine config data
function setup_hostfile() {

if [ "$DLWS_ROLE_NAME" = "ps" ];then
  if [ ! -f $HOST_CONFIG_FILE ];then touch $HOST_CONFIG_FILE;fi
  cat $HOST_CONFIG_FILE >> /etc/hosts

  if [ ! -f $WORKER_IB_CONFIG_FILE ];then touch $WORKER_IB_CONFIG_FILE;fi
  #cat $WORKER_IB_CONFIG_FILE >> $SSH_IB_CONFIG_FILE
fi
}

##############################################################################
prepare_constants
setup_ib_config
setup_hostfile
