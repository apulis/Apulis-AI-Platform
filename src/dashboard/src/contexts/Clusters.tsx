import React from "react";
import TeamContext from "./Teams";
import UserContext from './User';
import _ from 'lodash';

export interface AvailbleGpuType {
  type: string;
  quota: number;
  perNode: number;
}
interface Context {
  clusters: any[];
  selectedCluster?: string;
  saveSelectedCluster(team: React.SetStateAction<string>): void;
  availbleGpu: AvailbleGpuType[];
  selectedTeam: string;
  userName?: string;
  getTeams(): void;
}

const Context = React.createContext<Context>({
  clusters: [],
  selectedCluster: '',
  saveSelectedCluster: function(team: React.SetStateAction<string>) {},
  availbleGpu: [],
  selectedTeam: '',
  userName: '',
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

export const Provider: React.FC = ({ children }) => {
  const { teams, selectedTeam, getTeams } = React.useContext(TeamContext);
  console.log('React.useContext(TeamContext)'),React.useContext(TeamContext)
  const { userName } = React.useContext(UserContext);
  const [clusters, setClusters] = React.useState<ClusterType[]>([]);
  const [selectedCluster, setSelectedCluster] = React.useState<string>('');
  const saveSelectedCluster = (cluster: React.SetStateAction<string>) => {
    setSelectedCluster(cluster);
  };
  React.useEffect( ()=>{
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
      value={{ clusters, selectedCluster, saveSelectedCluster, availbleGpu, selectedTeam, userName, getTeams }}
      children={children}
    />
  );
};
