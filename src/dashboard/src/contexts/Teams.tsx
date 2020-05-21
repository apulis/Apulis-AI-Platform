import React, { useContext, useEffect } from 'react';
import useFetch from "use-http";
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

interface Context {
  teams: any;
  selectedTeam: any;
  saveSelectedTeam(team: React.SetStateAction<string>): void;
  clusterId: string;
  saveClusterId(clusterId: React.SetStateAction<string>): void;
  permissionList: string[];
}

const Context = React.createContext<Context>({
  teams: [],
  selectedTeam: '',
  saveSelectedTeam: function(team: React.SetStateAction<string>) {},
  clusterId: '',
  saveClusterId: function(clusterId: React.SetStateAction<string>) {},
  permissionList: []
});

export default Context;
export const Provider: React.FC<{permissionList?: string[]}> = ({ children, permissionList = [] }) => {
  console.log('permissionList', permissionList)
  const fetchTeamsUrl = '/api/teams';
  const { addGroup } = useContext(ConfigContext);
  const [clusterId, setClusterId] = React.useState<string>('');
  const saveClusterId = (clusterId: React.SetStateAction<string>) => {
    setClusterId(clusterId);
  };
  const { data: teams } = useFetch(fetchTeamsUrl, { onMount: true });
  const [selectedTeam, setSelectedTeam] = React.useState<string>('');
  const saveSelectedTeam = (team: React.SetStateAction<string>) => {
    setSelectedTeam(team);
    localStorage.setItem('team',team.toString());
    if (clusterId && window.location.pathname.split(`${clusterId}/`)[1]) {
      window.location.href = `/jobs-v2/${clusterId}/`;
    } else {
      window.location.reload();
    }
  }
  useEffect(()=> {
    if (localStorage.getItem('team')) {
      setSelectedTeam((String)(localStorage.getItem('team')))
    } else {
      setSelectedTeam(_.map(teams, 'id')[0]);
    }
  },[teams])
  const EmptyTeam: React.FC = () => {
    const onClick = () => {
      window.open(addGroup, "_blank");
    }
    return (
      <Box display="flex">
        <Dialog open>
          <DialogTitle style={{ color: 'red' }}>
            {"warning"}
          </DialogTitle>
          <DialogContent>
            <DialogContentText>
              {"You are not an authorized user for this cluster. Please request to join a security group by following the button below."}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button onClick={onClick} color="primary">
              JOIN SG
            </Button>
          </DialogActions>
        </Dialog>
      </Box>
    )
  };
  // if (teams !== undefined && teams.length === 0) {
  //   return (

  //     <Context.Provider
  //       value={{ teams, selectedTeam ,saveSelectedTeam,WikiLink }}
  //       children={<EmptyTeam addGroupLink={addGroupLink} WikiLink={WikiLink}/>}
  //     />
  //   )
  // }
  return (
    <Context.Provider
      value={{ teams, selectedTeam, saveSelectedTeam, clusterId, saveClusterId, permissionList }}
      children={children}
    />
  );
};
