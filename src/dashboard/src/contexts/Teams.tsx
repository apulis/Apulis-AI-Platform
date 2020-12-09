import React, { useEffect, useState, SetStateAction, createContext, FC } from 'react';
import {
  Box, Button,
  Dialog, DialogActions,
  DialogContent,
  DialogContentText,
  DialogTitle
} from "@material-ui/core";
import _ from "lodash";
import ConfigContext from './Config';
import axios from 'axios';
import Loading from '../components/Loading';
import { useTranslation } from "react-i18next";

interface Context {
  teams: any;
  selectedTeam: any;
  saveSelectedTeam(team: SetStateAction<string>): void;
  clusterId: string;
  saveClusterId(clusterId: SetStateAction<string>): void;
  getTeams(): void;
  permissionList: string[];
  administrators: string[];
  userGroupPath?: string;
}

const Context = createContext<Context>({
  teams: [],
  selectedTeam: '',
  saveSelectedTeam: function(team: SetStateAction<string>) {},
  clusterId: '',
  permissionList: [],
  saveClusterId: function(clusterId: SetStateAction<string>) {},
  getTeams: function() {},
  administrators: []
});

interface ProviderProps {
  userGroupPath?: string;
}


export default Context;
export const Provider: React.FC<{permissionList?: string[], administrators?: string[]} & ProviderProps> = ({ children, permissionList = [], userGroupPath = '', administrators = [] }) => {
  const fetchTeamsUrl = '/api/teams';
  const [clusterId, setClusterId] = React.useState<string>('');
  const saveClusterId = (clusterId: React.SetStateAction<string>) => {
    setClusterId(clusterId);
  };
  const [teams, setTeams] = useState<{id: string; clusters: any[]}[] | undefined>(undefined);
  const [selectedTeam, setSelectedTeam] = useState<string>('');
  const { t } = useTranslation();
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
      if (res && res.data && JSON.stringify(res.data) !== JSON.stringify(teams)) {
        if (res.data.length === 0) {
          // console.error('没有可用的虚拟集群，请联系管理员添加')
        }
        setTeams(res.data);
      }
    })
  }

  const clearAuthInfo = async () => {
    delete localStorage.token;
    await axios.get('/authenticate/logout');
    window.location.href = userGroupPath + '/user/login?' + encodeURIComponent(window.location.href);
  }

  useEffect(() => {
    getTeams();
  }, [])

  useEffect(()=> {
    const currentTeam = localStorage.getItem('team');
    if (currentTeam) {
      if (teams?.find(t => t.id === currentTeam)) {
        setSelectedTeam(currentTeam);
      } else {
        const localTeam = teams && teams[0] && teams[0].id;
        if (localTeam) {
          localStorage.team = localTeam;
          setSelectedTeam(localTeam);
        }
      }
    } else {
      setSelectedTeam(_.map(teams, 'id')[0]);
    }
  }, [teams]);
  
  const EmptyTeam: FC = () => {
    return (
      <Box display="flex">
        <Dialog open>
          <DialogTitle style={{ color: 'red' }}>
            {t('teams.emptyTitleWarning')}
          </DialogTitle>
          <DialogContent>
            <DialogContentText>
              {t('teams.emptyTitleContent')}
            </DialogContentText>
            <DialogActions>
              <Button variant="contained" href={`mailto:${administrators[0]}`}>{t('teams.emptyTitleSendEmail')}</Button>
              <Button variant="contained" onClick={() => clearAuthInfo()}>{t('teams.emptyTitleSignout')}</Button>
            </DialogActions>
          </DialogContent>
        </Dialog>
      </Box>
    )
  };

  if (teams === undefined) {
    return (

      <Context.Provider
        value={{ teams, selectedTeam ,saveSelectedTeam, clusterId, saveClusterId, getTeams, permissionList, administrators }}
        children={<Loading />}
      />
    )
  }
  if (teams !== undefined && teams.length === 0) {
    return <Context.Provider
      value={{ teams, selectedTeam ,saveSelectedTeam, clusterId, saveClusterId, getTeams, permissionList, administrators  }}
      children={<EmptyTeam />}
    />;
  }
  return (
    <Context.Provider
      value={{ teams, selectedTeam, saveSelectedTeam, clusterId, saveClusterId, getTeams, permissionList, administrators }}
      children={children}
    />
  );
};
