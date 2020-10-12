import React from "react";
import { MenuItem, TextField } from "@material-ui/core";
import { BaseTextFieldProps } from "@material-ui/core/TextField";
import ClustersContext from "../../../contexts/Clusters";
import TeamsContext from "../../../contexts/Teams";
import useFetch from "use-http";
import _ from "lodash";
import { useTranslation } from "react-i18next";

interface ClusterSelectFieldProps {
  cluster: string | undefined;
  onClusterChange(value: string): void;
  gpuType?: string;
  onAvailbleGpuNumChange?(val1: number, val2: number): void;
  userName?: string | undefined;
}

const ClusterSelectField: React.FC<ClusterSelectFieldProps & BaseTextFieldProps> = (
  { cluster, onClusterChange, variant="standard", userName, ...props }
) => {
  const {t} = useTranslation();
  const { clusters,selectedCluster, saveSelectedCluster } = React.useContext(ClustersContext);
  const { selectedTeam } = React.useContext(TeamsContext);
  const fetchVcStatusUrl = `/api`;
  const[helperText, setHelperText] = React.useState('');

  const request = useFetch(fetchVcStatusUrl);
  const fetchVC = async () => {
    const response = await request.get(`/teams/${selectedTeam}/clusters/${selectedCluster}`);
    return response;
  }
  const onChange = React.useCallback(
    (event: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
      saveSelectedCluster(event.target.value);
    },
    [saveSelectedCluster]
  );
  const isEmpty = (obj: object) => {
    if (obj === undefined) return true;
    for(let key in obj) {
      if(obj.hasOwnProperty(key))
        return false;
    }
    return true;
  }
  React.useEffect(() => {
    fetchVC().then((res)=>{
      let clusterName = props.gpuType || '', allHasUsed = 0, myHasUsed = 0;
      if (!isEmpty(res)) {
        if (!clusterName) {
          clusterName = (String)(Object.keys(res['gpu_capacity'])[0]);
        }
      }
      if (res['user_status']?.length) {
        res['user_status'].forEach((i: any) => {
          if (i['userGPU'][clusterName]) {
            if (i.userName === userName) myHasUsed = i['userGPU'][clusterName];
            allHasUsed += i['userGPU'][clusterName];
          }
        })
      }
      const gpuCapacity = isEmpty(res) ? 0 : JSON.parse(res.metadata)[clusterName]?.user_quota || 0;
      const gpuAvailable = Math.min(Number(JSON.parse(res.quota)[clusterName] - allHasUsed), Number(gpuCapacity - myHasUsed));
      props.onAvailbleGpuNumChange && props.onAvailbleGpuNumChange(gpuCapacity, gpuAvailable);
      setHelperText(`${clusterName} (${gpuAvailable} / ${gpuCapacity} ${t('components.toUse')})`);
    })
    if (selectedCluster) {
      onClusterChange(selectedCluster);
    }
  }, [clusters, onClusterChange, selectedCluster, props.gpuType]);

  if (cluster === undefined) {
    return null;
  }

  return (
    <TextField
      select
      label={t('submission.cluster')}
      helperText={helperText}
      value={cluster}
      onChange={onChange}
      variant="filled"
      {...props}
    >
      {//const filterclusters = clusters.filter((cluster)=>(boolean)cluster["admin"]);
        clusters && _.map(clusters,'id').map(cluster => (
          <MenuItem key={cluster} value={cluster}>{cluster}</MenuItem>
        ))
      }
    </TextField>
  );
}

export default ClusterSelectField;