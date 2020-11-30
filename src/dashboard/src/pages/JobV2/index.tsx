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
  Icon, Dialog, DialogContent, DialogTitle, DialogActions, Button, TextField
} from '@material-ui/core';
import {
  ArrowBack
} from '@material-ui/icons';
import SwipeableViews from 'react-swipeable-views';
import { useSnackbar } from 'notistack';
import useFetch from 'use-http-2';
import { useForm } from "react-hook-form";
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
import { NameErrorText, NameReg, pollInterval } from '../../const';
import AuthContext from '../../contexts/Auth';
import { useTranslation } from "react-i18next";

interface RouteParams {
  clusterId: string;
  jobId: string;
}

interface ISaveImageDialogProps {
  open: boolean;
  onCancel?: () => void;
  onOk?: () => void;
}

export const SaveImageDialog: React.FC<ISaveImageDialogProps> = ({ open, onCancel, onOk }) => {
  const { handleSubmit, register, errors, setValue, setError, clearError } = useForm({ mode: "onBlur" });
  const { t } = useTranslation();
  const { clusterId, jobId } = useParams<RouteParams>();
  const onSubmit = async (values: {[props: string]: string}) => {
    const res = await axios.post(`/clusters/${clusterId}/save_image`, {
      ...values,
      jobId,
      isPrivate: true,
    })
    const { code, data } = res.data;
    onOk && onOk();
    if (code === 0) {
      const { duration } = data;
      if (duration) {
        message('success', t('jobV2.saveImageSuccessLeft') + duration + t('jobV2.saveImageSuccessRight'));
      } else {
        message('success', t('jobV2.successSubmitImage'))
      }
    }
  }
  return (
    <Dialog open={open}>
      <DialogTitle style={{}}>
        {t('jobV2.saveImageDialogTitle')}
      </DialogTitle>
      <DialogContent>
        <form onSubmit={handleSubmit(onSubmit)}>
          <TextField
            name="name"
            fullWidth
            label={t('jobV2.imageName')}
            variant="filled"
            error={Boolean(errors.name)}
            helperText={errors.name ? errors.name.message : ''}
            InputLabelProps={{ shrink: true }}
            inputProps={{ maxLength: 20 }}
            style={{ margin: '10px 0' }}
            inputRef={register({
              required: t('jobV2.imageNameRequired') as string,
              pattern: {
                value: NameReg,
                message: t('jobV2.imageNameReg'),
              }
            })}
          />
          <TextField
            name="version"
            fullWidth
            label={t('jobV2.imageVersion')}
            variant="filled"
            error={Boolean(errors.version)}
            helperText={errors.version ? errors.version.message : ''}
            InputLabelProps={{ shrink: true }}
            inputProps={{ maxLength: 20 }}
            style={{ margin: '10px 0' }}
            inputRef={register({
              required: t('jobV2.imageVersionRequired') as string,
              pattern: {
                value: NameReg,
                message: t('jobV2.imageNameReg')
              }
            })}
          />
          <TextField
            name="description"
            fullWidth
            label={t('jobV2.desc')}
            variant="filled"
            error={Boolean(errors.desc)}
            helperText={errors.desc ? errors.desc.message : ''}
            InputLabelProps={{ shrink: true }}
            inputProps={{ maxLength: 20 }}
            style={{ margin: '10px 0' }}
            inputRef={register({
            })}
          />
          <DialogActions>
            <Button variant="contained" onClick={() => onCancel && onCancel()}>
              {t('jobV2.cancel')}
            </Button>
            <Button variant="contained" type="submit" color="primary">
              {t('jobV2.confirm')}
            </Button>
          </DialogActions>

        </form>
      </DialogContent>
    </Dialog>
  )
}

const JobToolbar: FunctionComponent<{ manageable: boolean; isMyJob: boolean }> = ({ manageable, isMyJob }) => {
  const { clusterId } = useParams<RouteParams>();
  const [saveImageVisible, setSaveImageVisible] = useState<boolean>(false);
  const { accessible, admin, job } = useContext(Context);
  const { permissionList = [] } = useContext(AuthContext);
  const { supportEmail, approve, kill, saveImage, pause, resume } = useActions(clusterId);
  const canAction = (!isMyJob && permissionList.includes('VIEW_AND_MANAGE_ALL_USERS_JOB')) || isMyJob;
  const availableActions = useMemo(() => {
    const actions = [supportEmail];
    if (manageable && admin && canAction) actions.push(approve);
    if (manageable && canAction) actions.push(saveImage, pause, resume, kill);
    return actions;
  }, [manageable, admin, supportEmail, approve, kill, pause, resume, saveImage]);

  const actionButtons = availableActions.map((action, index) => {
    const { hidden, icon, tooltip, onClick } = action(job);
    const handleClick = (event: React.MouseEvent<HTMLButtonElement, MouseEvent>, type: string) => {
      if (type === 'save') {
        setSaveImageVisible(true);
        return;
      }
      onClick(event, job)
    }
    if (hidden) return null;
    return (
      <Tooltip key={index} title={tooltip as string}>
        <IconButton onClick={(e) => handleClick(e, icon as string)}>
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
      <SaveImageDialog open={saveImageVisible} onOk={() => setSaveImageVisible(false)} onCancel={() => setSaveImageVisible(false)} />
    </Toolbar>
  );
}

const ManagableJob: FunctionComponent<{ jobStatus: any }> = ({ jobStatus }) => {
  const { t } = useTranslation();
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
        <Tab label={t('jobV2.brief')} />
        <Tab label={t('jobV2.endpoints')} />
        <Tab label={t('jobV2.metrics')} />
        <Tab label={t('jobV2.console')} />
      </Tabs>
      <SwipeableViews
        index={index}
        onChangeIndex={onChangeIndex}
      >
        {index === 0 ? <Brief /> : <div />}
        {index === 1 ? <Endpoints jobStatus={jobStatus} /> : <div />}
        {index === 2 ? <Metrics /> : <div />}
        {index === 3 ? <Console /> : <div />}
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
        <Tab label="Brief" />
        <Tab label="Metrics" />
        <Tab label="Console" />
      </Tabs>
      <SwipeableViews
        index={index}
        onChangeIndex={onChangeIndex}
      >
        {index === 0 ? <Brief /> : <div />}
        {index === 1 ? <Metrics /> : <div />}
        {index === 2 ? <Console /> : <div />}
      </SwipeableViews>
    </>
  );
}

const JobContent: FunctionComponent = () => {
  const { t } = useTranslation();
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
      const key = enqueueSnackbar(`t('jobV2.failedToFetchClusterConfig')} ${clusterId}`, {
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
        message('error', `${t('jobV2.failedToFetchJob')} ${clusterId}/${jobId}`);
      })
  }

  const status = useMemo(() => job && job['jobStatus'], [job]);
  const previousStatus = usePrevious(status);
  if (previousStatus !== undefined && status !== previousStatus) {
    enqueueSnackbar(`${t('jobV2.jobIs')} ${t('components.' + status)} ${t('jobV2.now')}`, { variant: "info" });
  }

  if (cluster === undefined || job === undefined) {
    return <Loading />;
  }

  return (
    <Context.Provider value={{ cluster, accessible, admin, job }}>
      <Helmet title={`(${capitalize(job['jobStatus'])}) ${job['jobName']}`} />
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
    <JobContent key={key} />
  );
}

export default Job;
