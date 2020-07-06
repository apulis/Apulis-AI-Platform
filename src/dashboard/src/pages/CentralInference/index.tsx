import React, { useState, FC, useEffect, useMemo, useContext } from "react";
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, CircularProgress, TextField, 
  MenuItem, Tabs, Tab } from "@material-ui/core";
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate, renderId, renderStatus } from '../JobsV2/tableUtils';
import { useForm } from "react-hook-form";
import ClusterContext from '../../contexts/Clusters';
import TeamsContext from '../../contexts/Teams';
import useActions from "../../hooks/useActions";
import { NameReg, NameErrorText, pollInterval } from '../../const';
import axios from 'axios';
import message from '../../utils/message';
import SwipeableViews from 'react-swipeable-views';
import AuthContext from '../../contexts/Auth';
import useInterval from '../../hooks/useInterval';

const CentralInference: React.FC = () => {
  const { selectedCluster } = useContext(ClusterContext);
  const { selectedTeam } = useContext(TeamsContext);
  const [pageSize, setPageSize] = useState(10);
  const [jobs, setJobs] = useState<any[]>([]);
  const [allJobs, setAllJobs] = useState<any[]>([]);
  const [gpuDevice, setGpuDevice] = useState<any[]>([]);
  const [allSupportInference, setAllSupportInference] = useState<any[]>([]);
  const [framework, setFramework] = useState('');
  const [deviceType, setDeviceType] = useState('');
  const [gpuType, setGpuType] = useState('');
  const [modalFlag, setModalFlag] = useState(false);
  const [btnLoading, setBtnLoading] = useState(false);
  const [index, setIndex] = useState(0);
  const [selectInference, setSelectInference] = useState({
    image: [],
    device: []
  });
  const [allDevice, setAllDevice] = useState<{
    [props: string]: {
      capacity: number,
      deviceStr: string
    }
  }>({});
  const { currentRole = [], userName } = useContext(AuthContext);
  const isAdmin = currentRole.includes('System Admin');
  const { handleSubmit, register, getValues, errors, setValue, clearError, setError } = useForm({ mode: "onBlur" });
  const { kill } = useActions(selectedCluster, true);
  const _renderId = (job: any) => <p title={job.jobId} style={{maxWidth: 300}}>{job.jobId}</p>;
  const _renderURL = (job: any) => <p title={job['inference-url'] || '--'}>{job['inference-url'] || '--'}</p>;
  const _renderPath = (job: any) => <p title={job.jobParams.model_base_path || '--'} style={{maxWidth: 300}}>{job.jobParams.model_base_path || '--'}</p>;
  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'Id', type: 'string', field: 'jobId',
    render: _renderId, disableClick: true, sorting: false, cellStyle: {fontFamily: 'Lucida Console'}},
    { title: 'Inference Name', type: 'string', field: 'jobName', sorting: false},
    { title: 'Username', type: 'string', field: 'userName'},
    { title: 'Path', type: 'string', field: 'jobParams.model_base_path', sorting: false, render: _renderPath, cellStyle: {maxWidth: 300} },
    { title: 'Framework', type: 'string', field: 'jobParams.framework', sorting: false },
    { title: 'DeviceType', type: 'string', field: 'jobParams.device', sorting: false },
    { title: 'Status', type: 'string', field: 'jobStatus', sorting: false, render: renderStatus },
    { title: 'URL', type: 'string', field: 'inference-url', sorting: false, render: _renderURL }
  ], []);
  const options = useMemo<Options>(() => ({
    padding: 'dense',
    actionsColumnIndex: -1,
    pageSize
  }), [pageSize]);

  useEffect(() => {
    getData();
  }, [selectedCluster, selectedTeam, index]);

  useEffect(() => {
    setSelectInference(allSupportInference.find((i: { framework: any; }) => i.framework === framework));
  }, [framework]);

  const getData = () => {
    const jobOwner = index === 1 && isAdmin ? 'all' : userName;
    axios.get(`/clusters/${selectedCluster}/teams/${selectedTeam}/inferenceJobs?jobOwner=${jobOwner}&limit=999`)
      .then((res: { data: any; }) => {
        const { data } = res;
        const _data = index ? allJobs : jobs
        const temp1 = JSON.stringify(_data.map((i: { jobStatus: any; }) => i.jobStatus));
        const temp2 = JSON.stringify(data?.map((i: { jobStatus: any; }) => i.jobStatus));
        const temp3 = JSON.stringify(_data.map(i => i['inference-url']));
        const temp4 = JSON.stringify(data?.map((i: { [x: string]: any; }) => i['inference-url']));
        if (temp1 !== temp2 || temp3 !== temp4) {
          index ? setAllJobs(data) : setJobs(data);
        }
      }, () => {
        message('error', `Failed to fetch inferences from cluster: ${selectedCluster}`);
      })
  }

  useInterval(() => {
    getData();
  }, pollInterval);
 
  const onSubmit = async (val: any) => {
    setBtnLoading(true);
    const imageArr = selectInference.image;
    const image = imageArr[selectInference.device.findIndex(i => i === deviceType)];
    await axios.post(`/clusters/${selectedCluster}/teams/${selectedTeam}/postInferenceJob`, {
      ...val,
      framework: framework,
      device: deviceType,
      image: image,
      resourcegpu: deviceType === 'CPU' ? 0 : 1,
      gpuType: gpuType
    }).then((res: any) => {
      message('success', `Inference successfully！`);
      setModalFlag(false);
      index ? setIndex(0) : getData();
    },  () => {
      message('error', `Inference failed！`);
    })
    setBtnLoading(false);
  }

  const openModal = () => {
    setModalFlag(true);
    axios.get(`/${selectedCluster}/getAllSupportInference`).then((res: { data: any; }) => {
      const { data } = res;
      setAllSupportInference(data);
      setFramework(data[0].framework);
      setDeviceType(data[0].device[0]);
    });
    axios.get(`/${selectedCluster}/getAllDevice?userName=${userName}`).then((res: { data: any; }) => {
      setAllDevice(res.data);
      getGpuDevice(res.data);
    });
  }

  const getFrameworkOptions = () => {
    if (allSupportInference) {
      return allSupportInference.map((i: { framework: any; }) => <MenuItem value={i.framework}>{i.framework}</MenuItem>)
    }
    return null;
  }

  const getDeviceTypeOptions = () => {
    if (framework && selectInference) {
      const arr = selectInference.device;
      return arr.map((i: any) => <MenuItem value={i}>{i}</MenuItem>)
    }
    return null;
  }

  const getGpuTypeOptions = () => {
    if (allDevice && deviceType === 'GPU') {
      if (gpuDevice.length) {
        return gpuDevice.map((i: any) => <MenuItem value={i}>{i}</MenuItem>);
      } else {
        message('error', `No GPU nodes available！`);
        setDeviceType('');
        return null;
      }
    }
    return null;
  }

  const getGpuDevice = (data: any) => {
    let arr: string[] = [];
    const _data = data ? data : allDevice;
    Object.keys(_data).forEach(i => {
      if (_data[i].deviceStr === 'nvidia.com/gpu') arr.push(i);
    });
    setGpuDevice(arr);
  }

  const onDeviceTypeChange = (e: any) => {
    const val = e.target.value;
    setDeviceType(val);
    if (gpuDevice.length && val === 'GPU') setGpuType(gpuDevice[0]);
  }

  return (
    <div className="centralWrap">
      <Button variant="contained" color="primary" onClick={openModal}>
        New Inference
      </Button>
      <Tabs
        value={index}
        onChange={(e: any, val: any) => setIndex(Number(val))}
        variant="fullWidth"
        textColor="primary"
        indicatorColor="primary"
      >
        <Tab label="My Inference"/>
        <Tab label="All Inference"/>
      </Tabs>
      <SwipeableViews index={index}>
        {index === 0 && <MaterialTable
          title="My Inference"
          columns={columns}
          data={jobs}
          options={options}
          actions={[kill]}
          onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
        />}
        {index === 1 && isAdmin && <MaterialTable
          title="All Inference"
          columns={columns}
          data={allJobs}
          options={options}
          actions={[kill]}
          onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
        />}
      </SwipeableViews>
      {modalFlag && 
      <Dialog open={modalFlag} disableBackdropClick fullWidth>
        <DialogTitle>New Inference</DialogTitle>
        <form onSubmit={handleSubmit(onSubmit)}>
          <DialogContent dividers>
            <TextField
              label="Inference Name"
              name="jobName"
              fullWidth
              variant="filled"
              error={Boolean(errors.jobName)}
              helperText={errors.jobName ? errors.jobName.message : ''}
              InputLabelProps={{ shrink: true }}
              inputProps={{ maxLength: 20 }}
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Inference Name is required！',
                pattern: {
                  value: NameReg,
                  message: NameErrorText
                }
              })}
            />
            <TextField
              label="Path"
              name="model_base_path"
              fullWidth
              variant="filled"
              error={Boolean(errors.model_base_path)}
              helperText={errors.model_base_path ? errors.model_base_path.message : ''}
              InputLabelProps={{ shrink: true }}
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Path is required！',
              })}
            />
            <TextField
              select
              label="Framework"
              name="framework"
              fullWidth
              onChange={(e: { target: { value: any; }; }) => setFramework(e.target.value)}
              variant="filled"
              style={{ margin: '10px 0' }}
              value={framework}
              InputLabelProps={{ shrink: true }}
            >
              {getFrameworkOptions()}
            </TextField>
            {framework && <TextField
              select
              label="Device Type"
              name="device"
              fullWidth
              onChange={onDeviceTypeChange}
              variant="filled"
              value={deviceType}
              style={{ margin: '10px 0' }}
            >
              {getDeviceTypeOptions()}
            </TextField>}
            {deviceType === 'GPU' && <TextField
              select
              label="GPU Type"
              name="gpuType"
              fullWidth
              onChange={(e: { target: { value: any; }; }) => setGpuType(e.target.value)}
              variant="filled"
              value={gpuType}
              style={{ margin: '10px 0' }}
            >
              {getGpuTypeOptions()}
            </TextField>}
          </DialogContent>
          <DialogActions>
            <Button onClick={() => setModalFlag(false)} color="primary" variant="outlined">Cancel</Button>
            <Button type="submit" color="primary" disabled={btnLoading} variant="contained" style={{ marginLeft: 8 }}>
              {btnLoading && <CircularProgress size={20}/>}Submit
            </Button>
          </DialogActions>
        </form>
      </Dialog>}
    </div>
  ); 
};

export default CentralInference;
