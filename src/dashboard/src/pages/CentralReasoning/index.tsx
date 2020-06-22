import React, { useState, FC, useEffect, useMemo, useContext } from "react";
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, CircularProgress, TextField, 
  MenuItem, Tabs, Tab } from "@material-ui/core";
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate, renderId, renderStatus } from '../JobsV2/tableUtils';
import SelectTree from '../CommonComponents/SelectTree';
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

const CentralReasoning: React.FC = () => {
  const { selectedCluster, availbleGpu } = useContext(ClusterContext);
  const { selectedTeam } = useContext(TeamsContext);
  const [pageSize, setPageSize] = useState(10);
  const [jobs, setJobs] = useState<any[]>([]);
  const [allJobs, setAllJobs] = useState<any[]>([]);
  const [allSupportInference, setAllSupportInference] = useState<any[]>([]);
  const [framework, setFramework] = useState('');
  const [deviceType, setDeviceType] = useState('');
  const [modalFlag, setModalFlag] = useState(false);
  const [btnLoading, setBtnLoading] = useState(false);
  const [index, setIndex] = useState(0);
  const { currentRole = [], userName } = useContext(AuthContext);
  const isAdmin = currentRole.includes('System Admin');
  const { handleSubmit, register, getValues, errors, setValue, clearError, setError } = useForm({ mode: "onBlur" });
  const { kill } = useActions(selectedCluster);
  const actions = [kill];
  const _renderId = (job: any) => renderId(job, 0);
  const _renderURL = (job: any) => <p title={job['inference-url'] || '--'} style={{maxWidth: 300}}>{job['inference-url'] || '--'}</p>;
  const _renderPath = (job: any) => <p title={job.jobParams.model_base_path || '--'} style={{maxWidth: 250}}>{job.jobParams.model_base_path || '--'}</p>;
  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'Id', type: 'string', field: 'jobId',
    render: _renderId, disableClick: true, sorting: false, cellStyle: {fontFamily: 'Lucida Console'}},
    { title: 'Jobname', type: 'string', field: 'jobName', sorting: false},
    { title: 'Username', type: 'string', field: 'userName'},
    { title: 'Path', type: 'string', field: 'jobParams.model_base_path', sorting: false, render: _renderPath },
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

  const getData = () => {
    const jobOwner = index === 1 && isAdmin ? 'all' : userName;
    axios.get(`/clusters/${selectedCluster}/teams/${selectedTeam}/inferenceJobs?jobOwner=${jobOwner}&limit=999`)
      .then((res: { data: any; }) => {
        const { data } = res;
        const _data = index ? allJobs : jobs
        const temp1 = JSON.stringify(_data.map(i => i.jobStatus));
        const temp2 = JSON.stringify(data?.map((i: { jobStatus: any; }) => i.jobStatus));
        if (!(temp1 === temp2)) {
          index ? setAllJobs(data) : setJobs(data);
        }
      }, () => {
        message('error', `Failed to fetch jobs from cluster: ${selectedCluster}`);
      })
  }

  useInterval(() => {
    getData();
  }, pollInterval);
 
  const onSubmit = async (val: any) => {
    setBtnLoading(true);
    await axios.post(`/clusters/${selectedCluster}/teams/${selectedTeam}/postInferenceJob`, {
      ...val,
      framework: framework,
      device: deviceType,
      image: allSupportInference.find(i => i.framework === framework).image
    }).then((res) => {
      message('success', `Reasoning successfully！`);
      setModalFlag(false);
      index ? setIndex(0) : getData();
    },  () => {
      message('error', `Reasoning failed！`);
    })
    setBtnLoading(false);
  }

  const openModal = () => {
    setModalFlag(true);
    axios.get(`/${selectedCluster}/getAllSupportInference`)
    .then((res: { data: any; }) => {
      const { data } = res;
      setAllSupportInference(data);
      setFramework(data[0].framework);
      setDeviceType(data[0].device[0]);
    })
  }

  const getFrameworkOptions = () => {
    if (allSupportInference) {
      return allSupportInference.map((i: { framework: any; }) => <MenuItem value={i.framework}>{i.framework}</MenuItem>)
    } else {
      return null;
    }
  }

  const getDeviceTypeOptions = () => {
    if (framework) {
      const arr = allSupportInference.find(i => i.framework === framework).device;
      return arr.map((i: any) => <MenuItem value={i}>{i}</MenuItem>)
    } else {
      return null;
    }
  }

  return (
    <div className="centralWrap">
      <Button variant="contained" color="primary" onClick={openModal}>
        New Reasoning
      </Button>
      <Tabs
        value={index}
        onChange={(e: any, val: any) => setIndex(Number(val))}
        variant="fullWidth"
        textColor="primary"
        indicatorColor="primary"
      >
        <Tab label="My Reasoning Jobs"/>
        <Tab label="All Reasoning Jobs"/>
      </Tabs>
      <SwipeableViews index={index}>
        {index === 0 && <MaterialTable
          title="My Reasoning Jobs"
          columns={columns}
          data={jobs}
          options={options}
          actions={actions}
          onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
        />}
        {index === 1 && isAdmin && <MaterialTable
          title="All Reasoning Jobs"
          columns={columns}
          data={allJobs}
          options={options}
          actions={actions}
          onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
        />}
      </SwipeableViews>
      {modalFlag && 
      <Dialog open={modalFlag} disableBackdropClick fullWidth>
        <DialogTitle>New Reasoning</DialogTitle>
        <form onSubmit={handleSubmit(onSubmit)}>
          <DialogContent dividers>
            <TextField
              label="Job Name"
              name="jobName"
              fullWidth
              variant="filled"
              error={Boolean(errors.jobName)}
              helperText={errors.jobName ? errors.jobName.message : ''}
              InputLabelProps={{ shrink: true }}
              inputProps={{ maxLength: 20 }}
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Job Name is required！',
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
              onChange={e => setFramework(e.target.value)}
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
              onChange={e => setDeviceType(e.target.value)}
              variant="filled"
              value={deviceType}
              style={{ margin: '10px 0' }}
            >
              {getDeviceTypeOptions()}
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

export default CentralReasoning;
