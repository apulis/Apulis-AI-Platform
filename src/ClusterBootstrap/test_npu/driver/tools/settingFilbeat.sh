#!/bin/bash
# This script get ES config and start filebeat lab env process.
# Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
if [ -z "$1" ];then
  path="/usr/local/HiAI/driver"
else
  path="$1"
fi

template="$path/tools/filebeat_lab.yml"
log_file="/var/log/npu/toolchain/run/hiai_run_servers.log"
config_file="/var/log/npu/conf/slog/filebeat_lab.yml"

log() {
        cur_date=$(date +"%Y-%m-%d %H:%M:%S")
        echo "[driver] [$cur_date] ""$1" >> "${log_file}"
}

get_es_configuration() {
  ips=""
  for ip in $ip_list
  do
    ips+="\"$ip:9200\","
  done
  ips=${ips%,*}
  echo "setup.template.name: \"our_book\"" > tmp_es.txt
  echo "setup.template.pattern: \"our_book*\"" >> tmp_es.txt
  echo "output.elasticsearch:" >> tmp_es.txt
  echo "  hosts: [$ips]" >> tmp_es.txt
  echo "  pipeline: my-pipeline-id1" >> tmp_es.txt
  echo "  index: \"our_book\"" >> tmp_es.txt
  echo "  bulk_max_size: 5000" >> tmp_es.txt
  echo "  worker: 2" >> tmp_es.txt
}

if [ -d $log_file ]; then
  rm -rf $log_file
fi
touch $log_file
chmod 640 $log_file

if [ ! -d "$path" ] || [ ! -f "$template" ];then
  log "Filebeat configuration file not exist"
  exit 1
fi

log "try to setting filebeat_lab.yml"
# Check required files exist
if [ $(which Get_Server_Info.py 2>/dev/null | wc -l) -eq 0 ];then
  log "Get_Server_Info.py not found"
  exit 1
fi

if [ ! -f "/dlyupg/Contral_Plane_IpAddress.xls" ];then
  log echo "Contral_Plane_IpAddress.xls not found"
  exit 1
fi

# Get LogAnalysis server IP
ip_list=$(Get_Server_Info.py Log IP 2>/dev/null)
if [ -z "$ip_list" ];then
  # Use another server type to double check
  ip_list=$(Get_Server_Info.py LogAnalysis IP 2>/dev/null)
  if [ -z "$ip_list" ];then
    log "No LogAnalysis server found"
    exit 1
  fi
fi

# Setting filebeat ES server configuration
line_num_s=$(sed -n -e '/- Elasticsearch output -/=' "${template}")
line_num_e=$(sed -n -e '/- Logstash output -/=' "${template}")
start_line=$[line_num_s+1]
end=$[line_num_e-1]

# Update config, clear tmp files
sed "${start_line},${end}d" "${template}" > tmp.yml
get_es_configuration
sed -i "${line_num_s}rtmp_es.txt" tmp.yml

rm tmp_es.txt
mv tmp.yml "${config_file}"
chmod 640 "${config_file}"
log "filbeat_lab setting finished"
