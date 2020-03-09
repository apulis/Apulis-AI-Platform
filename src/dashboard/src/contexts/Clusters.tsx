import React from "react";
import TeamContext from "./Teams";
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
  availbleGpu?: AvailbleGpuType[];
}

const Context = React.createContext<Context>({
  clusters: [],
  selectedCluster: '',
  saveSelectedCluster: function(team: React.SetStateAction<string>) {},
  availbleGpu: []
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
  const { teams,selectedTeam } = React.useContext(TeamContext);
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
      value={{ clusters, selectedCluster, saveSelectedCluster, availbleGpu}}
      children={children}
    />
  );
};
