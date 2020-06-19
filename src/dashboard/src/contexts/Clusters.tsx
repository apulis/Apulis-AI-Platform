import React, { useContext, useState, useEffect, createContext, SetStateAction, FunctionComponent } from "react";
import TeamContext from "./Teams";
import UserContext from './User';
import AuthContext from './Auth';
import _ from 'lodash';

export interface AvailbleGpuType {
  type: string;
  quota: number;
  perNode: number;
}
interface Context {
  clusters: any[];
  selectedCluster: string;
  saveSelectedCluster(team: SetStateAction<string>): void;
  availbleGpu: AvailbleGpuType[];
  selectedTeam: string;
  userName?: string;
  permissionList?: string[];
  getTeams(): void;
}

const Context = createContext<Context>({
  clusters: [],
  selectedCluster: '',
  saveSelectedCluster: function(team: SetStateAction<string>) {},
  availbleGpu: [],
  selectedTeam: '',
  userName: '',
  permissionList: [],
  getTeams: function() {}
});

export default Context;

export interface ClusterType {
  id: string;
  admin: boolean;
  gpus: {
    [propsName: string]: {
      quota: number;
      perNode: number;
    };
  };
}

export const Provider: FunctionComponent = ({ children }) => {
  const { teams, selectedTeam, getTeams } = useContext(TeamContext);
  const { userName } = useContext(UserContext);
  const { permissionList = [] } = useContext(AuthContext);
  const [clusters, setClusters] = useState<ClusterType[]>([]);
  const [selectedCluster, setSelectedCluster] = useState<string>('');
  const saveSelectedCluster = (cluster: SetStateAction<string>) => {
    setSelectedCluster(cluster);
  };
  useEffect( ()=>{
    if (teams && selectedTeam) {
      const filterClusters = teams.filter((team: any) => team.id === selectedTeam);

      for (let filterCluster of filterClusters) {
        setClusters(filterCluster.clusters)
        setSelectedCluster(_.map((filterCluster.clusters),'id')[0]);
      }
    }

  },[selectedTeam, teams]);
  const availbleGpu: AvailbleGpuType[] = [];
  if (clusters.length) {
    const cluster = clusters.find(c => c.id === selectedCluster);
    if (cluster) {
      Object.keys(cluster.gpus).forEach(gpu => {
        availbleGpu.push({
          type: gpu,
          quota: cluster.gpus[gpu].quota,
          perNode: cluster.gpus[gpu].perNode,
        })
      })
    }
  }
  return (
    <Context.Provider
      value={{ clusters, selectedCluster, saveSelectedCluster, availbleGpu, selectedTeam, userName, getTeams, permissionList }}
      children={children}
    />
  );
};
