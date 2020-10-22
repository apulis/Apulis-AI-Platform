import React, {
  ChangeEvent,
  FunctionComponent,
  useCallback,
  useContext,
  useMemo,
  useState
} from 'react';
import {
  useHistory,
  useParams
} from 'react-router-dom';
import {
  Container,
  FormControl,
  InputLabel,
  Paper,
  Tabs,
  Tab,
  Toolbar
} from '@material-ui/core';
import SwipeableViews from 'react-swipeable-views';
import ClustersContext from '../../contexts/Clusters';
import ClusterSelector from '../../components/ClusterSelector';
import Loading from '../../components/Loading';
import ClusterContext from './ClusterContext';
import MyJobs from './MyJobs';
import AllJobs from './AllJobs';
import AuthzHOC from '../../components/AuthzHOC';
import { useTranslation } from "react-i18next";

interface RouteParams {
  clusterId: string;
}

const TabView: FunctionComponent = () => {
  const {t} = useTranslation();
  const [index, setIndex] = useState(Number(window.location.search.split('index=')[1]) || 0);
  const onChange = useCallback((event: ChangeEvent<{}>, value: any) => {
    setIndex(value as number);
  }, [setIndex]);
  const onChangeIndex = useCallback((index: number, prevIndex: number) => {
    setIndex(index);
  }, [setIndex]);
  return (
    <div>
      <AuthzHOC needPermission={['VIEW_ALL_USER_JOB', 'VIEW_AND_MANAGE_ALL_USERS_JOB']}>
        <Tabs
          value={index}
          onChange={onChange}
          variant="fullWidth"
          textColor="primary"
          indicatorColor="primary"
        >
          <Tab label={t('jobsV2.myJobs')}/>
          <Tab label={t('jobsV2.allJobs')}/>
        </Tabs>
      </AuthzHOC>
      <SwipeableViews
        index={index}
        onChangeIndex={onChangeIndex}
      >
        {index === 0 ? <MyJobs/> : <div/>}
        {index === 1 && <AuthzHOC needPermission={['VIEW_ALL_USER_JOB', 'VIEW_AND_MANAGE_ALL_USERS_JOB']}><AllJobs/></AuthzHOC>}
      </SwipeableViews>
    </div>
  );
}

const ClusterJobs: FunctionComponent<{ cluster: any }> = ({ cluster }) => {
  return (
    <ClusterContext.Provider value={{ cluster: cluster }}>
      <Paper elevation={2}>
        {cluster.admin ? <TabView/> : <MyJobs/>}
      </Paper>
    </ClusterContext.Provider>
  );
}

const Jobs: FunctionComponent = () => {
  const {t} = useTranslation();
  const { clusters } = useContext(ClustersContext);
  const history = useHistory();
  const { clusterId } = useParams<RouteParams>();
  const cluster = useMemo(() => {
    const currentCluster = clusters.find(cluster => cluster.id === clusterId);
    if (currentCluster) {
      return currentCluster;
    } else {
      return clusters[0];
    }
  }, [clusters, clusterId]);
  const onClusterChange = useCallback((cluster: any) => {
    history.replace(`/jobs-v2/${cluster.id}`)
  }, [history]);

  return (
    <>
      <Toolbar disableGutters>
        <FormControl fullWidth>
  <InputLabel>{t('jobsV2.chooseCluster')}</InputLabel>
          <ClusterSelector defaultId={clusterId} onChange={onClusterChange}/>
        </FormControl>
      </Toolbar>
      {
        cluster !== undefined
          ? <ClusterJobs key={cluster.id} cluster={cluster}/>
          : <Loading/>
      }
    </>
  );
};

export default Jobs;
