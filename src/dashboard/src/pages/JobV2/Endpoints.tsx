import React, {
  FunctionComponent,
  ChangeEvent,
  FormEvent,
  useCallback,
  useContext,
  useEffect,
  useRef,
  useState,
  useMemo
} from 'react';
import { useParams } from 'react-router-dom';
import {
  Box,
  FormGroup,
  FormControlLabel,
  IconButton,
  InputAdornment,
  List,
  ListItem,
  ListItemText,
  Switch,
  TextField,
  Typography,
  Chip
} from '@material-ui/core';
import { Send, Info } from '@material-ui/icons';
import useFetch from 'use-http-2';
import { useSnackbar } from 'notistack';
import Loading from '../../components/Loading';
import CopyableTextListItem from '../../components/CopyableTextListItem';
import Context from './Context';
import { useForm } from "react-hook-form";
import { OneInteractivePortsMsg, pollInterval } from '../../const';
import message from '../../utils/message';
import axios from 'axios';
import useInterval from '../../hooks/useInterval';
import UserContext from '../../contexts/User';
import AuthContext from '../../contexts/Auth';

interface RouteParams {
  clusterId: string;
  jobId: string;
}

const EndpointListItem: FunctionComponent<{ endpoint: any }> = ({ endpoint }) => {
  const { cluster, job } = useContext(Context);
  const [ portInfoShow, setPortInfoShow ] = useState(true);
  if (endpoint.status !== "running") return null;
  if (endpoint.name === 'ssh') {
    const identify = `${cluster['workStorage'].replace(/^file:\/\//i, '//')}/${job['jobParams']['workPath']}/.ssh/id_rsa`
    const host = `${endpoint['nodeName']}.${endpoint['domain']}`;
    const task = job['jobParams']['jobtrainingtype'] === 'PSDistJob' ? endpoint['podName'].split('-').pop() : '';
    const command = `ssh -i ${identify} -p ${endpoint['port']} ${endpoint['username']}@${host}` + ` [Password: ${endpoint['password'] ? endpoint['password'] : ''}]`
    return <CopyableTextListItem primary={`SSH${task ? ` to ${task}` : ''}`} secondary={command}/>;
  }
  let url;
  if (endpoint.name == "ipython" || endpoint.name === 'tensorboard'){
    url = `http://${endpoint['nodeName']}.${endpoint['domain']}/endpoints/${endpoint['port']}/`
  }
  else{
    url = `http://${endpoint['nodeName']}.${endpoint['domain']}:${endpoint['port']}/`
  }
  if (endpoint.name === 'ipython') {
    return (
      <ListItem button component="a" href={url} target="_blank">
        <ListItemText primary="Jupyter" secondary={url}/>
      </ListItem>
    );
  }
  if (endpoint.name === 'tensorboard') {
    return (
      <ListItem button component="a" href={url} target="_blank">
        <ListItemText primary="TensorBoard" secondary={url}/>
      </ListItem>
    );
  }

  return (
    <ListItem button component="a" href={url} target="_blank">
      <ListItemText secondary={url}
        // primary={<p>Port {endpoint['podPort']} （<span>The server needs to use <code>/endpoints/{endpoint['podPort']}/</code> as the root path</span>）</p>} />
        primary={<p>Port {endpoint['podPort']}</p>} />
    </ListItem>
  );
}

const EndpointsList: FunctionComponent<{ endpoints: any[], setPollTime: any }> = ({ endpoints, setPollTime }) => {
  const sortedEndpoints = useMemo(() => {
    const nameOrders = ['ssh', 'ipython', 'tensorboard'].reverse();
    let flag = null;
    endpoints.map(i => { if (i.status !== 'running') flag = pollInterval; });
    setPollTime(flag);
    return endpoints.filter((endpoint) => {
      return endpoint.status === 'running';
    }).sort((endpointA, endpointB) => {
      const nameOrderA = nameOrders.indexOf(endpointA['name']);
      const nameOrderB = nameOrders.indexOf(endpointB['name']);
      if (nameOrderA !== nameOrderB) {
        return nameOrderB - nameOrderA;
      }
      if (endpointA.name === 'ssh') {
        return String(endpointA['podName']).localeCompare(String(endpointB['podName']));
      }
      return endpointA['port'] - endpointB['port'];
    });
  }, [endpoints]);
  return (
    <List dense style={{ paddingBottom: 20 }}>
      {sortedEndpoints.map((endpoint, idx) => {
        return <EndpointListItem key={endpoint.id} endpoint={endpoint} />
      })}
    </List>
  )
};

const EndpointsController: FunctionComponent<{ endpoints: any[], setPollTime: any }> = ({ endpoints, setPollTime }) => {
  const { job } = useContext(Context);
  const { userName } = useContext(UserContext);
  const { permissionList = [] } = useContext(AuthContext);
  const { jobStatus } = job;
  const canAction = (job.userName === userName || (job.userName !== userName && permissionList.includes('VIEW_AND_MANAGE_ALL_USERS_JOB')));
  const disabled = jobStatus === 'error' || jobStatus === 'killed' || jobStatus === 'failed' || jobStatus === 'finished' || jobStatus === 'killing' || !canAction;
  const { clusterId, jobId } = useParams<RouteParams>();
  const { enqueueSnackbar } = useSnackbar();
  const ssh = useMemo(() => {
    return endpoints.some((endpoint) => endpoint.name === 'ssh');
  }, [endpoints]);
  const ipython = useMemo(() => {
    return endpoints.some((endpoint) => endpoint.name === 'ipython');
  }, [endpoints]);
  const tensorboard = useMemo(() => {
    return endpoints.some((endpoint) => endpoint.name === 'tensorboard');
  }, [endpoints]);
  const { post } =
    useFetch(`/api/clusters/${clusterId}/jobs/${jobId}/endpoints`,
    [clusterId, jobId]);
  const portInput = useRef<HTMLInputElement>();

  const onChange = useCallback((name: string) => (event: ChangeEvent<{}>, value: boolean) => {
    if (value === false) return;
    const _name = name === 'iPython' ? 'Jupyter' : name;
    enqueueSnackbar(`Enabling ${_name}...`);
    post({
      endpoints: [name.toLowerCase()]
    }).then(() => {
      enqueueSnackbar(`${_name} enabled`, { variant: 'success' })
    }, () => {
      enqueueSnackbar(`Failed to enable ${_name}`, { variant: 'error' })
    });
  }, [post, enqueueSnackbar]);

  const onSubmit = (data: any) => {
    if (!data.interactivePorts) {
      setError('interactivePorts', 'validate', 'Interactive Port is required！');
      return;
    }
    const port = Number(data.interactivePorts);
    for (const i of endpoints) {
      const { status, podPort, name } = i;
      if (status === 'running' && name !== 'ssh' && name !== 'ipython' && name !== 'tensorboard' && podPort === port) {
        enqueueSnackbar(`Already has port ${port}！`, { variant: 'error' });
        return;
      }
    }
    enqueueSnackbar(`Exposing port ${port}...`);
    post({
      endpoints: [{
        name: `port-${port}`,
        podPort: port
      }]
    }).then(() => {
      enqueueSnackbar(`Port ${port} exposed`, { variant: 'success' });
      setValue('interactivePorts', '');
      setPollTime(pollInterval);
    }, () => {
      enqueueSnackbar(`Failed to expose port ${port}`, { variant: 'error' });
    });
  };

  const [iconInfoShow, setIconInfoShow] = useState(false);
  const { handleSubmit, register, errors, setError, setValue } = useForm({ mode: "onBlur" });
  const validateInteractivePorts = (val: string) => {
    if (val) {
      let flag = true;
      const arr = val.split(',');
      if (arr.length > 1) {
        flag = false;
      } else {
        flag = Number(val) >= 40000 && Number(val) <= 49999 && Number.isInteger(Number(val));
      }
      !flag && setError('interactivePorts', 'validate', OneInteractivePortsMsg);
      return flag;
    }
    return true;
  }

  return (
    <Box px={2}>
      <FormGroup aria-label="position" row>
        <FormControlLabel
          checked={ssh || undefined}
          disabled={ssh || disabled}
          control={<Switch/>}
          label="SSH"
          onChange={onChange('SSH')}
        />
        <FormControlLabel
          checked={ipython || undefined}
          disabled={ipython || disabled}
          control={<Switch/>}
          label="Jupyter"
          onChange={onChange('iPython')}
        />
        <FormControlLabel
          checked={tensorboard || undefined}
          disabled={tensorboard || disabled}
          control={<Switch/>}
          label="TensorBoard"
          onChange={onChange('Tensorboard')}
        />
        <Info fontSize="small" onClick={() => setIconInfoShow(!iconInfoShow)} style={{ marginTop: 8, cursor: 'pointer' }}/>
      </FormGroup>
      {iconInfoShow && <Chip icon={<Info/>}
        label={<p>TensorBoard will listen on directory<code> ~/tensorboard/$DLWS_JOB_ID/logs </code>inside docker container.</p>}
      />}
      {/* <AuthzHOC needPermission={'"MANAGE_ALL_USERS_JOB"'}></AuthzHOC> */}
      <Box pt={1} pb={2} component="form" onSubmit={handleSubmit(onSubmit)}>
        <TextField
          fullWidth
          label="New Interactive Port"
          disabled={disabled}
          name="interactivePorts"
          error={Boolean(errors.interactivePorts)}
          defaultValue={''}
          helperText={errors.interactivePorts ? errors.interactivePorts.message || OneInteractivePortsMsg : ''}
          inputRef={register({
            validate: val => validateInteractivePorts(val)
          })}
          InputProps={{
            endAdornment: !disabled && (
              <InputAdornment position="end">
                <IconButton type="submit">
                  <Send/>
                </IconButton>
              </InputAdornment>
            )
          }}
        />
      </Box>
    </Box>
  )
};

const Endpoints: FunctionComponent<{ jobStatus: any }> = ({ jobStatus }) => {
  const { clusterId, jobId } = useParams<RouteParams>();
  const { enqueueSnackbar, closeSnackbar } = useSnackbar();
  const { job } = useContext(Context);
  const [endpoints, setEndpoints] = useState<any[]>([]);
  const [pollTime, setPollTime] = useState<any>(pollInterval);

  useEffect(() => {
    getData();
  }, [clusterId, jobId]);

  useInterval(() => {
    getData();
  }, pollTime);

  const getData = () => {
    axios.get(`/clusters/${clusterId}/jobs/${jobId}/endpoints`)
      .then(res => {
        const { data } = res;
        const eLen = endpoints.length;
        const dLen = data ? data.length : 0;
        if (jobStatus === 'error' || jobStatus === 'failed' || jobStatus === 'finished' || jobStatus === 'killing' || jobStatus === 'killed') setPollTime(null);
        if (eLen !== dLen) {
          setEndpoints(data);
        } else if (eLen && dLen) {
          for (const m of data) {
            for (const n of endpoints) {
              if (m.id === n.id && m.status !== n.status) {
                setEndpoints(data);
                return;
              }
            }
          }
        }
      }, () => {
        message('error', `Failed to fetch job endpoints: ${clusterId}/${jobId}`);
      })
  }

  if (endpoints === undefined) {
    return <Loading/>;
  }
  return (
    <>
      {job['jobStatus'] === 'running' && <EndpointsList endpoints={endpoints} setPollTime={setPollTime} />}
      <EndpointsController endpoints={endpoints} setPollTime={setPollTime} />
    </>
  );
};

export default Endpoints;
