import React, { useContext, useEffect, useState, SetStateAction, createContext, FC } from 'react';
import {
  Box, Button,
  Dialog, DialogActions,
  DialogContent,
  DialogContentText,
  DialogTitle
} from "@material-ui/core";
import _ from "lodash";
import ConfigContext from './Config';
import ClustersContext from '../contexts/Clusters';
import axios from 'axios';
import Loading from '../components/Loading';

interface Context {
  teams: any;
  selectedTeam: any;
  saveSelectedTeam(team: SetStateAction<string>): void;
  clusterId: string;
  saveClusterId(clusterId: SetStateAction<string>): void;
  getTeams(): void;
  permissionList: string[];
}

const Context = createContext<Context>({
  teams: [],
  selectedTeam: '',
  saveSelectedTeam: function(team: SetStateAction<string>) {},
  clusterId: '',
  permissionList: [],
  saveClusterId: function(clusterId: SetStateAction<string>) {},
  getTeams: function() {},
});

export default Context;
export const Provider: React.FC<{permissionList?: string[]}> = ({ children, permissionList = [] }) => {
  const fetchTeamsUrl = '/api/teams';
  const [clusterId, setClusterId] = React.useState<string>('');
  const saveClusterId = (clusterId: React.SetStateAction<string>) => {
    setClusterId(clusterId);
  };
  const [teams, setTeams] = useState<{id: string; clusters: any[]}[]>([]);
  const [selectedTeam, setSelectedTeam] = useState<string>('');
  const saveSelectedTeam = (team: SetStateAction<string>) => {
    setSelectedTeam(team);
    localStorage.setItem('team',team.toString());
    if (clusterId && window.location.pathname.split(`${clusterId}/`)[1]) {
      window.location.href = `/jobs-v2/${clusterId}/`;
    } else {
      window.location.reload();
    }
  }

  const getTeams = () => {
    axios.get('/teams').then(res => {
      setTeams(res.data);
    })
  }

  useEffect(() => {
    getTeams();
  }, [])

  useEffect(()=> {
    console.log('tema', teams)
    const currentTeam = localStorage.getItem('team');
    if (currentTeam) {
      if (teams.find(t => t.id === currentTeam)) {

        setSelectedTeam(currentTeam);
      } else {
        setSelectedTeam(teams && teams[0] && teams[0].id)
      }
    } else {
      setSelectedTeam(_.map(teams, 'id')[0]);
    }
  },[teams]);
  
  // const EmptyTeam: FC = () => {
  //   return (
  //     <Box display="flex">
  //       <Dialog open>
  //         <DialogTitle style={{ color: 'red' }}>
  //           {"warning"}
  //         </DialogTitle>
  //         <DialogContent>
  //           <DialogContentText>
  //             {"You are not an authorized user for this cluster. Please request to join a security group by following the button below."}
  //           </DialogContentText>
  //         </DialogContent>
  //         <DialogActions>
  //           <Button onClick={onClick} color="primary">
  //             JOIN SG
  //           </Button>
  //         </DialogActions>
  //       </Dialog>
  //     </Box>
  //   )
  // };
  if (!teams || teams.length === 0) {
    return null;
  }
  if (teams !== undefined && teams.length === 0) {
    return (

      <Context.Provider
        value={{ teams, selectedTeam ,saveSelectedTeam, clusterId, saveClusterId, getTeams, permissionList  }}
        children={<Loading />}
      />
    )
  }
  return (
    <Context.Provider
      value={{ teams, selectedTeam, saveSelectedTeam, clusterId, saveClusterId, getTeams, permissionList }}
      children={children}
    />
  );
};
