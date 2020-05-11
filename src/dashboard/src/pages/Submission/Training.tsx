import React, { useState, useEffect, useMemo, useContext, useCallback } from "react";
import {
  Card,
  CardHeader,
  CardContent,
  CardActions,
  Grid,
  Container,
  TextField,
  FormControlLabel,
  Checkbox,
  Button,
  Divider,
  Chip,
  Collapse,
  Typography,
  Table,
  TableHead,
  TableRow,
  TableCell,
  TableBody,
  Switch,
  MenuItem,
  SvgIcon, useMediaQuery,
  Dialog, DialogActions, DialogContent, DialogTitle
} from "@material-ui/core";
import axios from 'axios';
import Tooltip from '@material-ui/core/Tooltip';
import { Info, Delete, Add } from "@material-ui/icons";
import { withRouter } from "react-router";
import IconButton from '@material-ui/core/IconButton';
import { useSnackbar } from 'notistack';
import useFetch from "use-http";
import { join } from 'path';
import _ from "lodash";
import ClusterSelectField from "./components/ClusterSelectField";
import UserContext from "../../contexts/User";
import ClustersContext from '../../contexts/Clusters';
import TeamsContext from "../../contexts/Teams";
import theme, { Provider as MonospacedThemeProvider } from "../../contexts/MonospacedTheme";
import {BarChart, Bar, XAxis, YAxis, CartesianGrid, LabelList} from "recharts";
import {green, grey, red} from "@material-ui/core/colors";
import {DLTSDialog} from "../CommonComponents/DLTSDialog";
import {
  SUCCESSFULSUBMITTED,
  SUCCESSFULTEMPLATEDELETE, SUCCESSFULTEMPLATEDSAVE
} from "../../Constants/WarnConstants";
import {DLTSSnackbar} from "../CommonComponents/DLTSSnackbar";
import message from '../../utils/message';
import { NameReg, NameErrorText } from '../../const';
import './Training.less';
import { useForm } from "react-hook-form";

interface EnvironmentVariable {
  name: string;
  value: string;
}

const sanitizePath = (path: string) => {
  path = join('/', path);
  path = join('.', path);
  return path;
}
const Training: React.ComponentClass = withRouter(({ history }) => {
  const { selectedCluster, saveSelectedCluster, availbleGpu } = useContext(ClustersContext);
  const { userName, uid } = useContext(UserContext);
  const { teams, selectedTeam }= useContext(TeamsContext);
  const { enqueueSnackbar } = useSnackbar()
  //const team = 'platform';
  const [showGPUFragmentation, setShowGPUFragmentation] = useState(false)
  const [grafanaUrl, setGrafanaUrl] = useState('');
  const [name, setName] = useState("");
  const [gpuFragmentation, setGpuFragmentation] = useState<any[]>([]);
  const team = useMemo(() => {
    if (teams == null) return;
    if (selectedTeam == null) return;
    return teams.filter((team: any) => team.id === selectedTeam)[0];
  }, [teams, selectedTeam]);
  const cluster = useMemo(() => {
    if (team == null) return;
    if (selectedCluster == null) return;
    return team.clusters.filter((cluster: any) => cluster.id === selectedCluster)[0];
  }, [team, selectedCluster]);
  const gpuModel = useMemo(() => {
    if (cluster == null) return;
    return Object.keys(cluster.gpus)[0];
  }, [cluster]);
  const [gpuType, setGpuType] = useState(availbleGpu[0] ? availbleGpu[0].type : '');
  const [gpusPerNode, setGpusPerNode] = useState(0)
  const [templates, setTemplates] = useState([{name: '', json: ''}]);
  const [type, setType] = useState("RegularJob");
  const [preemptible, setPreemptible] = useState(false);
  const [workers, setWorkers] = useState(0);
  const [image, setImage] = useState("");
  const [command, setCommand] = useState("");
  const [interactivePorts, setInteractivePorts] = useState("");
  const [ssh, setSsh] = useState(false);
  const [ipython, setIpython] = useState(false);
  const [tensorboard, setTensorboard] = useState(false);
  const [advanced, setAdvanced] = useState(false);
  const [accountName, setAccountName] = useState("");
  const [accountKey, setAccountKey] = useState("");
  const [containerName, setContainerName] = useState("");
  const [mountPath, setMountPath] = useState("");
  const [mountOptions, setMountOptions] = useState("");
  const [workPath, setWorkPath] = useState("");
  const [dockerRegistry, setDockerRegistry] = useState("");
  const [dockerUsername, setDockerUsername] = useState("");
  const [dockerPassword, setDockerPassword] = useState("");
  const [enableWorkPath, setEnableWorkPath] = useState(true);
  const [dataPath, setDataPath] = useState("");
  const [enableDataPath, setEnableDataPath] = useState(true);
  const [jobPath, setJobPath] = useState("");
  const [enableJobPath, setEnableJobPath] = useState(true);
  const [showSaveTemplate, setSaveTemplate] = useState(false);
  const [deleteModal, setDeleteModal] = useState(false);
  const [environmentVariables, setEnvironmentVariables] = useState<EnvironmentVariable[]>([]);
  const onEnvironmentVariableNameChange = useCallback(
    (index: number) => (event: React.ChangeEvent<HTMLInputElement>) => {
      const newEnvironmentVariables = environmentVariables.slice()
      environmentVariables[index].name = event.target.value;
      setEnvironmentVariables(newEnvironmentVariables);
    },
    [environmentVariables]
  );
  const onEnvironmentVariableValueChange = useCallback(
    (index: number) => (event: React.ChangeEvent<HTMLInputElement>) => {
      const newEnvironmentVariables = environmentVariables.slice()
      environmentVariables[index].value = event.target.value;
      setEnvironmentVariables(newEnvironmentVariables);
    },
    [environmentVariables]
  );
  const onRemoveEnvironmentVariableClick = useCallback(
    (index: number) => () => {
      const newEnvironmentVariables = environmentVariables.slice();
      newEnvironmentVariables.splice(index, 1);
      setEnvironmentVariables(newEnvironmentVariables)
    },
    [environmentVariables]
  )
  const onAddEnvironmentVariableClick = useCallback(() => {
    setEnvironmentVariables(
      environmentVariables.concat(
        [{ name: "", value: "" }]));
  }, [environmentVariables]);
  const [database, setDatabase] = useState(false);
  const [saveTemplateName, setSaveTemplateName] = useState("");
  const [selectDelTPName, setSelectDelTPName] = useState('');
  const [saveTemplateDatabase, setSaveTemplateDatabase] = useState("user");
  const [iconInfoShow, setIconInfoShow] = useState(false);
  const { handleSubmit, register, errors, setValue } = useForm({ mode: "onBlur" });
  const [gpus, setGpus] = useState(0);
  const onSaveTemplateClick = async () => {
    if (!saveTemplateName) {
      message('error', 'Need input template name')
      return
    }
    try {
      let plugins: any = {};
      plugins['blobfuse'] = [];
      let blobfuseObj: any = {};
      blobfuseObj['accountName'] = accountName || '';
      blobfuseObj['accountKey'] = accountKey || '';
      blobfuseObj['containerName'] = containerName || '';
      blobfuseObj['mountPath'] = mountPath || '';
      blobfuseObj['mountOptions'] = mountOptions || '';
      plugins['blobfuse'].push(blobfuseObj);
      plugins['imagePull'] = [];
      let imagePullObj: any = {};
      imagePullObj['registry'] = dockerRegistry;
      imagePullObj['username'] = dockerUsername;
      imagePullObj['password'] = dockerPassword;
      plugins['imagePull'].push(imagePullObj);

      const template = {
        name,
        type,
        gpus,
        workers,
        image,
        command,
        workPath,
        enableWorkPath,
        dataPath,
        enableDataPath,
        jobPath,
        enableJobPath,
        environmentVariables,
        ssh,
        ipython,
        tensorboard,
        plugins,
        gpuType,
        preemptible,
        interactivePorts
      };
      const url = `/teams/${selectedTeam}/templates/${saveTemplateName}?database=${saveTemplateDatabase}`;
      await axios.put(url, template);
      setSaveTemplate(true);
      // window.location.reload();
    } catch (error) {
      enqueueSnackbar('Failed to save the template', {
        variant: 'error',
      })
      console.error(error);
    }
  };
  const [showDeleteTemplate, setShowDeleteTemplate] = useState(false);
  const onDeleteTemplateClick = async () => {
    if (!selectDelTPName) {
      message('error', 'Need select one template')
      return
    }
    try {
      const url = `/teams/${selectedTeam}/templates/${selectDelTPName}`;
      await axios.delete(url);
      setShowDeleteTemplate(true);
      setDeleteModal(false);
      setSelectDelTPName('');
      getTemplates();
    } catch (error) {
      enqueueSnackbar('Failed to delete the template', {
        variant: 'error',
      })
      console.error(error);
    }
  }
  const [json, setJson] = useState('');
  const [selectTPName, setSelectTPName] = useState('None (Apply a Template)');
  const onTemplateChange = (e: React.ChangeEvent<HTMLSelectElement>) => {
    const val = e.target.value;
    if (val === 'None (Apply a Template)') {
      setName("");
      setValue('jobName', '');
      setType("RegularJob");
      setGpus(0);
      setValue('setGpus', 0);
      setWorkers(0);
      setImage("");
      setValue('image', '');
      setCommand("");
      setValue('command', '');
      setWorkPath("");
      setEnableWorkPath(true);
      setDataPath("");
      setEnableDataPath(true);
      setJobPath("");
      setEnableJobPath(true);
      setEnvironmentVariables([]);
      setSsh(false);
      setIpython(false);
      setTensorboard(false);
      setGpuType(availbleGpu![0].type || '')
      setPreemptible(false);
      setValue('interactivePorts', '');
    } else {
      const {
        name,
        type,
        gpus,
        workers,
        image,
        command,
        workPath,
        enableWorkPath,
        dataPath,
        enableDataPath,
        jobPath,
        enableJobPath,
        environmentVariables,
        ssh,
        ipython,
        tensorboard,
        plugins,
        gpuType,
        preemptible,
        interactivePorts
      } = JSON.parse(templates.find(i => i.name === val)!.json);
      if (name !== undefined) {
        setName(name);
        setValue('jobName', name);
      }
      if (type !== undefined) setType(type);
      if (gpus !== undefined) {
        setGpus(gpus);
        setValue('gpus', gpus);
      }
      if (workers !== undefined) setWorkers(workers);
      if (image !== undefined) {
        setImage(image);
        setValue('image', image);
      }
      if (command !== undefined) {
        setCommand(command);
        setValue('command', command);
      }
      if (workPath !== undefined) setWorkPath(workPath);
      if (enableWorkPath !== undefined) setEnableWorkPath(enableWorkPath);
      if (dataPath !== undefined) setDataPath(dataPath);
      if (enableDataPath !== undefined) setEnableDataPath(enableDataPath);
      if (jobPath !== undefined) setJobPath(jobPath);
      if (enableJobPath !== undefined) setEnableJobPath(enableJobPath);
      if (environmentVariables !== undefined) setEnvironmentVariables(environmentVariables);
      if (ssh !== undefined) setSsh(ssh);
      if (ipython !== undefined) setIpython(ipython);
      if (tensorboard !== undefined) setTensorboard(tensorboard);
      if (gpuType !== undefined) setGpuType(gpuType);
      if (preemptible !== undefined) setPreemptible(preemptible);
      if (interactivePorts !== undefined) {
        setInteractivePorts(interactivePorts);
        setValue('interactivePorts', interactivePorts);
      }
      if (plugins === undefined) {
        setAccountName("");
        setAccountKey("");
        setContainerName("");
        setMountPath("");
        setMountOptions("");
        setDockerRegistry("")
        setDockerUsername("")
        setDockerPassword("")
      }
      if (plugins !== undefined) {
        if (plugins.hasOwnProperty("blobfuse") && Array.isArray(plugins['blobfuse'])) {
          let blobfuseObj = plugins['blobfuse'][0];
          setAccountName(blobfuseObj['accountName']);
          setAccountKey(blobfuseObj['accountKey']);
          setContainerName(blobfuseObj['containerName']);
          setMountPath(blobfuseObj['mountPath']);
          setMountOptions(blobfuseObj['mountOptions']);
        }

        if (plugins.hasOwnProperty('imagePull') && Array.isArray(plugins['imagePull'])) {
          let imagePullObj = plugins['imagePull'][0];
          setDockerRegistry(imagePullObj['registry'])
          setDockerUsername(imagePullObj['username'])
          setDockerPassword(imagePullObj['password'])
        }
      }
      setJson(templates.find(i => i.name === val)!.json);
    }
    setSelectTPName(val);
  }
  const {
    data: postJobData,
    loading: postJobLoading,
    error: postJobError,
    post: postJob,
  } = useFetch('/api');
  const {
    data: postEndpointsData,
    loading: postEndpointsLoading,
    error: postEndpointsError,
    post: postEndpoints,
  } = useFetch('/api');
  const [open, setOpen] = useState(false);
  const onSubmit = (data: any) => {
    let plugins: any = {};
    plugins['blobfuse'] = [];
    let blobfuseObj: any = {};
    blobfuseObj['accountName'] = accountName || '';
    blobfuseObj['accountKey'] = accountKey || '';
    blobfuseObj['containerName'] = containerName || '';
    blobfuseObj['mountPath'] = mountPath || '';
    blobfuseObj['mountOptions'] = mountOptions || '';
    plugins['blobfuse'].push(blobfuseObj);
    plugins['imagePull'] = [];
    let imagePullObj: any = {};
    imagePullObj['registry'] = dockerRegistry
    imagePullObj['username'] = dockerUsername
    imagePullObj['password'] = dockerPassword
    plugins['imagePull'].push(imagePullObj)
    const job: any = {
      userName: userName,
      userId: uid,
      jobType: 'training',
      gpuType: gpuType,
      vcName: selectedTeam,
      containerUserId: 0,
      jobName: name,
      jobtrainingtype: type,
      preemptionAllowed: preemptible ? 'True' : 'False',
      image,
      cmd: command,
      workPath: sanitizePath(enableWorkPath ? workPath : ''),
      enableworkpath: enableWorkPath,
      dataPath: sanitizePath(enableDataPath ? dataPath : ''),
      enabledatapath: enableDataPath,
      jobPath: sanitizePath(enableJobPath ? jobPath : ''),
      enablejobpath: enableJobPath,
      env: environmentVariables,
      hostNetwork : type === 'PSDistJob',
      isPrivileged : type === 'PSDistJob',
      interactivePorts: interactivePorts,
      plugins: plugins,
    };
    let totalGpus = gpus;
    if (type === 'PSDistJob') {
      job.numps = 1;
      job.resourcegpu = gpusPerNode;
      job.numpsworker = workers;
      totalGpus = gpusPerNode * workers;
    } else {
      job.resourcegpu = gpus;
    }
    if (type === 'PSDistJob') {
      let workersNeeded = workers;
      for (const { metric, value } of gpuFragmentation) {
        if (Number(metric['gpu_available']) >= gpusPerNode) {
          workersNeeded -= (Number(value[1]) || 0);
        }
        if (workersNeeded <= 0) break;
      }
      if (workersNeeded > 0) {
        if (!window.confirm('There won\'t be enough workers match your request.\nProceed?')) {
          return;
        }
      }
    }
    postJob(`/clusters/${selectedCluster}/jobs`, job);
  };

  const jobId = React.useRef<string>();
  const fetchGrafanaUrl = `/api/clusters`;
  const request = useFetch(fetchGrafanaUrl);
  const fetchGrafana = async () => {
    const result = await request.get(`/${selectedCluster}`);
    if (result) {
      const { grafana } = result
      setGrafanaUrl(grafana);
    }
  }

  useEffect(() => {
    if (postJobData == null) return;
    if (postJobData.error) {
      enqueueSnackbar(postJobData.error, {
        variant: 'error'
      });
      return
    }

    jobId.current = postJobData['jobId'];
    const endpoints = [];
    for (const port of interactivePorts.split(',')) {
      const portNumber = Number(port)
      if (portNumber >= 40000 && portNumber <= 49999) {
        endpoints.push({
          name: `port-${portNumber}`,
          podPort: portNumber
        });
      }
    }
    if (ssh) endpoints.push('ssh');
    if (ipython) endpoints.push('ipython');
    if (tensorboard) endpoints.push('tensorboard');
    
    if (endpoints.length > 0) {
      postEndpoints(`/clusters/${selectedCluster}/jobs/${jobId.current}/endpoints`, { endpoints });
    } else {
      history.push(`/jobs-v2/${selectedCluster}/${jobId.current}`);
    }
  }, [postJobData]);

  const validateInteractivePorts = (val: string) => {
    if (val) {
      let flag = true;
      if (val.split(',').length > 1) {
        val.split(',').forEach(n => {
          if (Number(n) < 40000 || Number(n) > 49999) flag = false;
        });
      } else {
        flag = Number(val) >= 40000 && Number(val) <= 49999;
      }
      return flag;
    }
    return true;
  }

  const validateNumDevices = (val: string) => {
    if (val) {
      const _val = Number(val);
      return (!(_val < 0) && Number.isInteger(_val) && !(_val > gpusPerNode));
    }
    return true;
  }

  useEffect(() => {
    getTemplates();
  }, [selectedTeam]);

  const getTemplates = () => {
    axios.get(`/teams/${selectedTeam}/templates`)
      .then(res => {
        setTemplates(res.data)
      })
  }

  useEffect(() => {
    fetchGrafana()
    if (postEndpointsData) {
      setOpen(true);
      setTimeout(()=>{
        history.push(`/jobs-v2/${selectedCluster}/${jobId.current}`);
      }, 2000)

    }
  }, [history, postEndpointsData, selectedCluster, selectedTeam])

  useEffect(() => {
    if (postJobError) {
      enqueueSnackbar('Job submission failed', {
        variant: 'error',
      })
    }
  }, [postJobError])

  useEffect(() => {
    if (postEndpointsError) {
      enqueueSnackbar('Enable endpoints failed', {
        variant: 'error',
      })
    }
  }, [postEndpointsError])

  const handleClose = () => {
    setOpen(false)
    setSaveTemplate(false)
    setShowDeleteTemplate(false)
  }

  useEffect(() => {
    if (!grafanaUrl) return;
    let getNodeGpuAva = `${grafanaUrl}/api/datasources/proxy/1/api/v1/query?`;
    const params1 = new URLSearchParams({
      // query:'count_values("gpu_available", k8s_node_gpu_available)'
      query: `count_values("device_available",k8s_node_device_available{deviceType="${gpuType}"})`
    });
    const params2 = new URLSearchParams({
      query: `sum(pai_node_count{deviceType!="${gpuType}"})`
    })
    fetch(getNodeGpuAva+params1).then(async (res1: any) => {
      fetch(getNodeGpuAva+params2).then(async (res2: any) => {
        let data1 = await res1.json();
        let data2 = await res2.json();
        let result1 = data1.data.result, result2 = data2.data.result;
        if (result1.length && result2.length) {
          let sortededResult = [{metric: {device_available: "0"}, value: result2[0].value}]
          sortededResult[0].value = result2[0].value;
          result1.forEach((i: { metric: { device_available: string }, value: Array<[]> }) => {
            if (i.metric.device_available === '0') {
              sortededResult[0].value[1] = (Number(sortededResult[0].value[1]) + Number(i.value[1])).toString();
            } else {
              sortededResult.push(i);
            }
          });
          sortededResult = sortededResult.sort((a: any, b: any)=>a['metric']['gpu_available'] - b['metric']['gpu_available']);
          setGpuFragmentation(sortededResult)
        }
      })
    })
  }, [grafanaUrl, gpuType])

  const isDesktop = useMediaQuery(theme.breakpoints.up("sm"));
  const showMessage = (open: boolean,showDeleteTemplate: boolean,showSaveTemplate: boolean) => {
    let message = '';
    if (open) {
      message = SUCCESSFULSUBMITTED;
    }
    if (showDeleteTemplate) {
      message = SUCCESSFULTEMPLATEDELETE;
    }
    if (showSaveTemplate) {
      message = SUCCESSFULTEMPLATEDSAVE;
    }
    return message;
  }
  const renderCustomizedLabel = (props: any) => {
    const { x, y, width, height, value } = props;
    const radius = 10;
    return (
      <g>
        <circle cx={x + width / 2} cy={y - radius} r={radius} fill="#fff" />
        <text x={x + width / 2} y={y - radius} fill="#000" textAnchor="middle" dominantBaseline="middle">
          {value}
        </text>
      </g>
    );
  };
  const styleSnack={backgroundColor:showDeleteTemplate ? red[400] : green[400]};

  return (
    <Container maxWidth={isDesktop ? 'lg' : 'xs'}>
      <div className="training-wrap" >
        <DLTSDialog open={showGPUFragmentation}
          message={null}
          handleClose={() => setShowGPUFragmentation(false)}
          handleConfirm={null} confirmBtnTxt={null} cancelBtnTxt={null}
          title={"View Cluster GPU Status Per Node"}
          titleStyle={{color:grey[400]}}
        >
          <BarChart width={500} height={600} data={gpuFragmentation}  margin={{top: 20}}>
            <CartesianGrid strokeDasharray="10 10"/>
            <XAxis dataKey={"metric['gpu_available']"} label={{value: 'Available gpu count', position:'insideBottomLeft'}}>
            </XAxis>
            <YAxis label={{value: 'Node count', angle: -90, position: 'insideLeft'}} allowDecimals={false} />
            <Bar dataKey="value[1]" fill="#8884d8" >
              <LabelList dataKey="value[1]" content={renderCustomizedLabel} />
            </Bar>
          </BarChart>
        </DLTSDialog>
        <form onSubmit={handleSubmit(onSubmit)}>
          <Card>
            <CardHeader title="Submit Training Job"/>
            <Divider/>
            <CardContent>
              <Grid
                container
                wrap="wrap"
                spacing={1}
              >
                <Grid item xs={12} sm={6}>
                  <ClusterSelectField
                    data-test="cluster-item"
                    fullWidth
                    cluster={selectedCluster}
                    gpuType={gpuType}
                    onClusterChange={saveSelectedCluster}
                    onAvailbleGpuNumChange={(value) => {setGpusPerNode(value)}}
                  />
                  <Tooltip title="View Cluster GPU Status Per Node">
                    <IconButton color="secondary" size="small" onClick={() => setShowGPUFragmentation(true)} aria-label="delete">
                      <SvgIcon>
                        <path d="M5 9.2h3V19H5zM10.6 5h2.8v14h-2.8zm5.6 8H19v6h-2.8z"/><path fill="none" d="M0 0h24v24H0z"/>
                      </SvgIcon>
                    </IconButton>
                  </Tooltip>
                </Grid>
                <Grid item xs={12} sm={6}>
                  <TextField
                    label="Job Name"
                    name="jobName"
                    fullWidth
                    variant="filled"
                    defaultValue={name}
                    error={Boolean(errors.jobName)}
                    onChange={e => setName(e.target.value)}
                    helperText={errors.jobName ? errors.jobName.message : ''}
                    InputLabelProps={{ shrink: true }}
                    inputRef={register({
                      required: 'Job Name is required！',
                      pattern: {
                        value: NameReg,
                        message: NameErrorText
                      }
                    })}
                  />
                </Grid>
                <Grid item xs={12}>
                  <TextField
                    disabled={!Array.isArray(templates)}
                    select
                    label="Job Template"
                    fullWidth
                    variant="filled"
                    value={selectTPName}
                    onChange={onTemplateChange}
                  >
                    <MenuItem value={'None (Apply a Template)'} divider>None (Apply a Template)</MenuItem>
                    {Array.isArray(templates) && templates.sort((a,b)=>a.name.localeCompare(b.name)).map(({ name, json }: any, index: number) => (
                      <MenuItem key={index} value={name}>{name}</MenuItem>
                    ))}
                  </TextField>
                </Grid>
                <Grid item xs={12} sm={6}>
                  <TextField
                    select
                    label="Job Type"
                    fullWidth
                    variant="filled"
                    value={type}
                    onChange={e => setType(e.target.value as string)}
                  >
                    <MenuItem value="RegularJob">Regular Job</MenuItem>
                    {/* <MenuItem value="PSDistJob">Distirbuted Job</MenuItem>
                    <MenuItem value="InferenceJob">Inference Job</MenuItem> */}
                  </TextField>
                </Grid>
                <Grid item xs={12} sm={6}>
                  <TextField
                    select
                    label="Preemptible Job"
                    fullWidth
                    variant="filled"
                    value={String(preemptible)}
                    onChange={e => setPreemptible(e.target.value === 'true')}
                  >
                    <MenuItem value="false">NO</MenuItem>
                    <MenuItem value="true">YES</MenuItem>
                  </TextField>
                </Grid>
                <Grid item xs={12} sm={6}>
                  <TextField
                    select
                    label="Device Type"
                    fullWidth
                    variant="filled"
                    value={String(gpuType)}
                    onChange={e => setGpuType(e.target.value)}
                  >
                    {availbleGpu?.map(gpu => (
                      <MenuItem value={gpu.type}>{gpu.type}</MenuItem>
                    ))}
                  </TextField>
                </Grid>
                {(type === 'RegularJob' ||  type === 'InferenceJob') && (
                  <Grid item xs={6}>
                    <TextField
                      name="gpus"
                      label="Number of Device"
                      fullWidth
                      variant="filled"
                      defaultValue={gpus}
                      error={Boolean(errors.gpus)}
                      onChange={e => setGpus(Number(e.target.value))}
                      helperText={errors.gpus ? `Must be a positive integer from 0 to ${gpusPerNode}` : ''}
                      inputRef={register({
                        validate: val => validateNumDevices(val)
                      })}
                    />
                  </Grid>
                )}
                { type === 'PSDistJob'  && (
                  <Grid item xs={12} sm={6}>
                    <TextField
                      type="number"
                      label="Number of Nodes"
                      fullWidth
                      variant="filled"
                      value={workers}
                      onChange={e => setWorkers(Number(e.target.value))}
                    />
                  </Grid>
                )}
                { type === 'PSDistJob' && (
                  <Grid item xs={12} sm={6}>
                    <TextField
                      disabled
                      type="number"
                      label="Total Number of Device"
                      value = {workers * gpusPerNode}
                      fullWidth
                      variant="filled"
                    />
                  </Grid>
                )}
                <Grid item xs={12}>
                  <TextField
                    label="Docker Image"
                    fullWidth
                    variant="filled"
                    defaultValue={image}
                    InputLabelProps={{ shrink: true }}
                    name="image"
                    onChange={e => setImage(e.target.value)}
                    error={Boolean(errors.image)}
                      helperText={errors.image ? errors.image.message : ''}
                      inputRef={register({
                        required: 'Docker Image is required！'
                      })}
                  />
                </Grid>
                <Grid item xs={12}>
                  <MonospacedThemeProvider>
                    <TextField
                      multiline
                      label="Command"
                      name="command"
                      fullWidth
                      variant="filled"
                      rows="10"
                      defaultValue={command}
                      InputLabelProps={{ shrink: true }}
                      onChange={e => setCommand(e.target.value)}
                      error={Boolean(errors.command)}
                      helperText={errors.command ? errors.command.message : ''}
                      inputRef={register({
                        required: 'Command is required！'
                      })}
                    />
                  </MonospacedThemeProvider>
                </Grid>
                <Grid item xs={12}>
                  <TextField
                    label="Interactive Ports"
                    placeholder="40000 - 49999. Separated by comma."
                    fullWidth
                    variant="filled"
                    rows="10"
                    name="interactivePorts"
                    defaultValue={interactivePorts}
                    InputLabelProps={{ shrink: true }}
                    onChange={e => setInteractivePorts(e.target.value)}
                    error={Boolean(errors.interactivePorts)}
                    helperText={errors.interactivePorts ? '40000 - 49999. Separated by comma.' : ''}
                    inputRef={register({
                      validate: val => validateInteractivePorts(val)
                    })}
                  />
                </Grid>
                <Grid item xs={4} container justify="center">
                  <FormControlLabel
                    control={<Checkbox />}
                    label="SSH"
                    checked={ssh}
                    onChange={(e, checked) => setSsh(checked)}
                  />
                </Grid>
                <Grid item xs={4} container justify="center">
                  <FormControlLabel
                    control={<Checkbox />}
                    label="iPython"
                    checked={ipython}
                    onChange={(e, checked) => setIpython(checked)}
                  />
                </Grid>
                <Grid item xs={4} container justify="center" className="icon-grid">
                  <FormControlLabel
                    control={<Checkbox />}
                    label="Tensorboard"
                    checked={tensorboard}
                    onChange={(e, checked) => setTensorboard(checked)}
                  />
                  <Info fontSize="small" onClick={() => setIconInfoShow(!iconInfoShow)} />
                </Grid>
                {iconInfoShow && <Grid item xs={12} container justify="flex-end">
                  <Chip
                    icon={<Info/>}
                    label="Tensorboard will listen on directory ~/tensorboard/<JobId>/logs inside docker container."
                  />
                </Grid>}
              </Grid>
            </CardContent>
            <Collapse in={advanced}>
              <Divider/>
              <CardContent>
                <Typography component="div" variant="h6" >Custom Docker Registry</Typography>
                <Grid
                  container
                  wrap="wrap"
                  spacing={1}
                  align-items-xs-baseline
                >
                  <Grid item xs={12}>
                    <TextField
                      value={dockerRegistry}
                      onChange={e => setDockerRegistry(e.target.value)}
                      label="Registry"
                      fullWidth
                      variant="filled"
                    />
                  </Grid>
                  <Grid item xs={12}>
                    <TextField
                      value={dockerUsername}
                      onChange={e => setDockerUsername(e.target.value)}
                      label="Username"
                      fullWidth
                      variant="filled"
                    />
                  </Grid>
                  <Grid item xs={12}>
                    <TextField
                      value={dockerPassword}
                      onChange={e => setDockerPassword(e.target.value)}
                      label="Password"
                      fullWidth
                      variant="filled"
                    />
                  </Grid>
                </Grid>
              </CardContent>
              <CardContent>
                <Typography component="span" variant="h6">Mount Directories</Typography>
                <Table size="small">
                  <TableHead>
                    <TableRow>
                      <TableCell>Path in Container</TableCell>
                      <TableCell>Path on Host Machine / Storage Server</TableCell>
                      <TableCell align="center">Enable</TableCell>
                    </TableRow>
                  </TableHead>
                  <TableBody>
                    <TableRow>
                      <TableCell>/work</TableCell>
                      <TableCell>
                        <TextField
                          label="Work Path"
                          fullWidth
                          margin="dense"
                          variant="filled"
                          value={workPath}
                          onChange={e => setWorkPath(e.target.value)}
                        />
                      </TableCell>
                      <TableCell align="center">
                        <Switch
                          value={enableWorkPath}
                          checked={enableWorkPath}
                          onChange={(e, checked) => setEnableWorkPath(checked)}
                        />
                      </TableCell>
                    </TableRow>
                    <TableRow>
                      <TableCell>/data</TableCell>
                      <TableCell>
                        <TextField
                          label="Data Path"
                          fullWidth
                          margin="dense"
                          variant="filled"
                          value={dataPath}
                          onChange={e => setDataPath(e.target.value)}
                        />
                      </TableCell>
                      <TableCell align="center">
                        <Switch
                          value={enableDataPath}
                          checked={enableDataPath}
                          onChange={(e, checked) => setEnableDataPath(checked)}
                        />
                      </TableCell>
                    </TableRow>
                    <TableRow>
                      <TableCell>/job</TableCell>
                      <TableCell>
                        <TextField
                          label="Job Path"
                          fullWidth
                          margin="dense"
                          variant="filled"
                          value={jobPath}
                          onChange={e => setJobPath(e.target.value)}
                        />
                      </TableCell>
                      <TableCell align="center">
                        <Switch
                          value={enableJobPath}
                          checked={enableJobPath}
                          onChange={(e, checked) => setEnableJobPath(checked)}
                        />
                      </TableCell>
                    </TableRow>
                  </TableBody>
                </Table>
              </CardContent>
              <CardContent>
                <Typography component="span" variant="h6">Environment Variables</Typography>
                <Table size="small">
                  <TableHead>
                    <TableRow>
                      <TableCell>Name</TableCell>
                      <TableCell>Value</TableCell>
                      <TableCell/>
                    </TableRow>
                  </TableHead>
                  <TableBody>
                    {environmentVariables.map(({ name, value }, index) => (
                      <TableRow key={index}>
                        <TableCell>
                          <TextField
                            label="Environment Variable Name"
                            fullWidth
                            margin="dense"
                            variant="filled"
                            value={name}
                            onChange={onEnvironmentVariableNameChange(index)}
                          />
                        </TableCell>
                        <TableCell>
                          <TextField
                            label="Environment Variable Value"
                            fullWidth
                            margin="dense"
                            variant="filled"
                            value={value}
                            onChange={onEnvironmentVariableValueChange(index)}
                          />
                        </TableCell>
                        <TableCell align="center">
                          <IconButton size="small" color="secondary" onClick={onRemoveEnvironmentVariableClick(index)}>
                            <Delete/>
                          </IconButton>
                        </TableCell>
                      </TableRow>
                    ))}
                    <TableRow>
                      <TableCell/>
                      <TableCell/>
                      <TableCell align="center">
                        <IconButton size="small" color="secondary" onClick={onAddEnvironmentVariableClick}>
                          <Add/>
                        </IconButton>
                      </TableCell>
                    </TableRow>
                  </TableBody>
                </Table>
              </CardContent>
            </Collapse>
            <Collapse in={database}>
              <Divider/>
              <CardContent>
                <Typography component="span" variant="h6">Template Management</Typography>
                <Grid container wrap="wrap" spacing={1}>
                  <Grid item xs={12} sm={6}>
                    <TextField
                      label="Template name"
                      fullWidth
                      variant="filled"
                      value={saveTemplateName}
                      onChange={e => setSaveTemplateName(e.target.value)}
                    />
                  </Grid>
                  <Grid item xs>
                    <TextField
                      label="Scope"
                      select
                      fullWidth
                      variant="filled"
                      value={saveTemplateDatabase}
                      onChange={e => setSaveTemplateDatabase((e.target.value) as string)}
                    >
                      <MenuItem value="user">user</MenuItem>
                      <MenuItem value="team">team</MenuItem>
                    </TextField>
                  </Grid>
                  <Button type="button" color="primary" onClick={onSaveTemplateClick}>Save</Button>
                  <Button type="button" color="secondary" onClick={() => setDeleteModal(true)}>Delete</Button>
                </Grid>
              </CardContent>
            </Collapse>
            <Divider/>
            <CardActions>
              <Grid item xs={12} container justify="space-between">
                <Grid item xs container>
                  <Button type="button" color="secondary"  onClick={() => setAdvanced(!advanced)}>Advanced</Button>
                  <Button type="button" color="secondary"  onClick={() => setDatabase(!database)}>Template</Button>
                </Grid>
                <Button type="submit" color="primary" variant="contained" disabled={postJobLoading || postEndpointsLoading || open }>Submit</Button>
              </Grid>
            </CardActions>
          </Card>
        </form>
        {deleteModal && 
        <Dialog open={deleteModal} maxWidth='xs' fullWidth onClose={() => setDeleteModal(false)}>
          <DialogTitle>Delete Template</DialogTitle>
          <DialogContent>
            <TextField
              disabled={!Array.isArray(templates)}
              select
              label="Select Template"
              fullWidth
              variant="filled"
              value={selectDelTPName}
              onChange={e => setSelectDelTPName(e.target.value)}
            >
              {Array.isArray(templates) && templates.sort((a,b)=>a.name.localeCompare(b.name)).map(({ name, json }: any, index: number) => (
                <MenuItem key={index} value={name}>{name}</MenuItem>
              ))}
            </TextField>
          </DialogContent>
          <DialogActions>
            <Button onClick={() => setDeleteModal(false)} color="primary">
              Cancel
            </Button>
            <Button onClick={onDeleteTemplateClick} color="secondary">
            Delete
            </Button>
          </DialogActions>
        </Dialog>}
        <DLTSSnackbar message={showMessage(open,showDeleteTemplate,showSaveTemplate)}
          open={open || showSaveTemplate || showDeleteTemplate}
          style={styleSnack}
          handleWarnClose={handleClose}
          autoHideDuration={1000}
        />
      </div>
    </Container>
  );
});

export default Training;
