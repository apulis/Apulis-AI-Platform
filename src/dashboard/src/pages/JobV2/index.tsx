import React, {
  FunctionComponent,
  ChangeEvent,
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useState
} from 'react';
import { capitalize } from 'lodash';
import { usePrevious } from 'react-use';
import {
  Link,
  useParams,
  withRouter
} from 'react-router-dom';
import Helmet from 'react-helmet';
import {
  Container,
  IconButton,
  Paper,
  Tabs,
  Tab,
  Toolbar,
  Typography,
  Tooltip,
  Icon
} from '@material-ui/core';
import {
  ArrowBack
} from '@material-ui/icons';
import SwipeableViews from 'react-swipeable-views';
import { useSnackbar } from 'notistack';
import useFetch from 'use-http-2';
import UserContext from '../../contexts/User';
import ClustersContext from '../../contexts/Clusters';
import TeamContext from '../../contexts/Teams';
import Loading from '../../components/Loading';
import useActions from '../../hooks/useActions';
import Context from './Context';
import Brief from './Brief';
import Endpoints from './Endpoints';
import Metrics from './Metrics';
import Console from './Console';
import axios from 'axios';
import message from '../../utils/message';
import useInterval from '../../hooks/useInterval';
import { pollInterval } from '../../const';
import AuthContext from '../../contexts/Auth';

interface RouteParams {
  clusterId: string;
  jobId: string;
}

const JobToolbar: FunctionComponent<{ manageable: boolean; isMyJob: boolean }> = ({ manageable, isMyJob }) => {
  const { clusterId } = useParams<RouteParams>();
  const { accessible, admin, job } = useContext(Context);
  const { permissionList = [] } = useContext(AuthContext);
  const { supportEmail, approve, kill, pause, resume } = useActions(clusterId);
  const canAction = (!isMyJob && permissionList.includes('VIEW_AND_MANAGE_ALL_USERS_JOB')) || isMyJob;
  const availableActions = useMemo(() => {
    const actions = [supportEmail];
    if (manageable && admin && canAction) actions.push(approve);
    if (manageable && canAction) actions.push(pause, resume, kill);
    return actions;
  }, [manageable, admin, supportEmail, approve, kill, pause, resume]);

  const actionButtons = availableActions.map((action, index) => {
    const { hidden, icon, tooltip, onClick } = action(job);
    if (hidden) return null;
    return (
      <Tooltip key={index} title={tooltip}>
        <IconButton onClick={(event) => onClick(event, job)}>
          <Icon>{icon}</Icon>
        </IconButton>
      </Tooltip>
    )
  })
  
  return (
    <Toolbar disableGutters variant="dense">
      {accessible && (
        <IconButton
          edge="start"
          color="inherit"
          component={Link}
          to={`/jobs-v2/${clusterId}/${window.location.search}`}
        >
          <ArrowBack />
        </IconButton>
      )}
      <Typography variant="h6" style={{ flexGrow: 1 }}>
        {job['jobName']}
      </Typography>
      {actionButtons}
    </Toolbar>
  );
}

const ManagableJob: FunctionComponent<{ jobStatus: any }> = ({ jobStatus }) => {
  const [index, setIndex] = useState(0);
  const onChange = useCallback((event: ChangeEvent<{}>, value: any) => {
    setIndex(value as number);
  }, [setIndex]);
  const onChangeIndex = useCallback((index: number, prevIndex: number) => {
    setIndex(index);
  }, [setIndex]);
  return (
    <>
      <Tabs
        value={index}
        onChange={onChange}
        variant="fullWidth"
        textColor="primary"
        indicatorColor="primary"
      >
        <Tab label="Brief"/>
        <Tab label="Endpoints"/>
        <Tab label="Metrics"/>
        <Tab label="Console"/>
      </Tabs>
      <SwipeableViews
        index={index}
        onChangeIndex={onChangeIndex}
      >
        {index === 0 ? <Brief/> : <div/>}
        {index === 1 ? <Endpoints jobStatus={jobStatus} /> : <div/>}
        {index === 2 ? <Metrics/> : <div/>}
        {index === 3 ? <Console/> : <div/>}
      </SwipeableViews>
    </>
  );
}

const UnmanagableJob: FunctionComponent = () => {
  const [index, setIndex] = useState(0);
  const onChange = useCallback((event: ChangeEvent<{}>, value: any) => {
    setIndex(value as number);
  }, [setIndex]);
  const onChangeIndex = useCallback((index: number, prevIndex: number) => {
    setIndex(index);
  }, [setIndex]);
  return (
    <>
      <Tabs
        value={index}
        onChange={onChange}
        variant="fullWidth"
        textColor="primary"
        indicatorColor="primary"
      >
        <Tab label="Brief"/>
        <Tab label="Metrics"/>
        <Tab label="Console"/>
      </Tabs>
      <SwipeableViews
        index={index}
        onChangeIndex={onChangeIndex}
      >
        {index === 0 ? <Brief/> : <div/>}
        {index === 1 ? <Metrics/> : <div/>}
        {index === 2 ? <Console/> : <div/>}
      </SwipeableViews>
    </>
  );
}

const JobContent: FunctionComponent = () => {
  const { clusterId, jobId } = useParams<RouteParams>();
  const { enqueueSnackbar, closeSnackbar } = useSnackbar();
  const { email, userName } = useContext(UserContext);
  const { clusters } = useContext(ClustersContext);
  const { saveClusterId } = useContext(TeamContext);
  const [pollTime, setPollTime] = useState<any>(pollInterval);

  saveClusterId(clusterId);
  const teamCluster = useMemo(() => {
    return clusters.filter((cluster) => cluster.id === clusterId)[0];
  }, [clusters, clusterId]);
  const accessible = useMemo(() => {
    return teamCluster !== undefined;
  }, [teamCluster]);
  const admin = useMemo(() => {
    return accessible && Boolean(teamCluster.admin);
  }, [accessible, teamCluster]);
  const [job, setJob] = useState<any>();
  const { error: clusterError, data: cluster } =
    useFetch(`/api/clusters/${clusterId}`, [clusterId]);
  const manageable = useMemo(() => {
    if (job === undefined) return false;
    if (admin === true) return true;
    if (job['userName'] === email) return true;
    return false;
  }, [job, admin, email]);

  useEffect(() => {
    if (clusterError !== undefined) {
      const key = enqueueSnackbar(`Failed to fetch cluster config: ${clusterId}`, {
        variant: 'error',
        persist: true
      });
      return () => {
        if (key !== null) closeSnackbar(key);
      }
    }
  }, [clusterError, enqueueSnackbar, closeSnackbar, clusterId, jobId]);

  useEffect(() => {
    getJob();
  }, [clusterId, jobId]);

  useInterval(() => {
    getJob();
  }, pollTime);

  const getJob = () => {
    axios.get(`/v2/clusters/${clusterId}/jobs/${jobId}`)
      .then(res => {
        const { data } = res;
        const { jobStatus } = data;
        const temp1 = JSON.stringify(job ? job.jobStatus : '');
        const temp2 = JSON.stringify(data ? jobStatus : '');
        if (jobStatus === 'error' || jobStatus === 'failed' || jobStatus === 'finished' || jobStatus === 'killing' || jobStatus === 'killed') setPollTime(null);
        if (!(temp1 === temp2)) setJob(data);
      }, () => {
        message('error', `Failed to fetch job: ${clusterId}/${jobId}`);
      })
  }

  const status = useMemo(() => job && job['jobStatus'], [job]);
  const previousStatus = usePrevious(status);
  if (previousStatus !== undefined && status !== previousStatus) {
    enqueueSnackbar(`Job is ${status} now.`, { variant: "info" });
  }

  if (cluster === undefined || job === undefined) {
    return <Loading/>;
  }

  return (
    <Context.Provider value={{ cluster, accessible, admin, job }}>
      <Helmet title={`(${capitalize(job['jobStatus'])}) ${job['jobName']}`}/>
      <Container fixed maxWidth="lg">
        <JobToolbar manageable={manageable} isMyJob={job.userName === userName} />
        <Paper elevation={2}>
          <>
            <ManagableJob jobStatus={job['jobStatus']} />
            {/* {manageable || <UnmanagableJob/>} */}
          </>
        </Paper>
      </Container>
    </Context.Provider>
  );
}

const Job: FunctionComponent = () => {
  const { clusterId, jobId } = useParams<RouteParams>();
  const key = useMemo(() => `${clusterId}/${jobId}`, [clusterId, jobId]);
  return (
    <JobContent key={key}/>
  );
}

export default Job;
