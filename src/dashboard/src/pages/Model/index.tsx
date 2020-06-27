import React, { useState, FC, useEffect, useMemo, useContext } from "react";
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, RadioGroup, Radio, CircularProgress,
  FormControlLabel, TextField, MenuItem } from "@material-ui/core";
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate } from '../JobsV2/tableUtils';
import './index.less';
import SelectTree from '../CommonComponents/SelectTree';
import { useForm } from "react-hook-form";
import ClusterContext from '../../contexts/Clusters';
import TeamsContext from '../../contexts/Teams';
import message from '../../utils/message';
import axios from 'axios';
import AuthContext from '../../contexts/Auth';
import useInterval from '../../hooks/useInterval';
import { NameReg, NameErrorText, pollInterval } from '../../const';

const Model: React.FC = () => {
  const { currentRole = [], userName } = useContext(AuthContext);
  const getDate = (item: any) => new Date(item.jobTime);
  const isAdmin = currentRole.includes('System Admin');
  const { selectedCluster } = useContext(ClusterContext);
  const { selectedTeam } = useContext(TeamsContext);
  const [pageSize, setPageSize] = useState(10);
  const [jobs, setJobs] = useState([]);
  const [type, setType] = useState('');
  const [fdInfo, setFdInfo] = useState({
    username: '',
    url: '',
    password: ''
  });
  const [convertionTypes, setConvertionTypes] = useState([]);
  const [modalFlag1, setModalFlag1] = useState(false);
  const [modalFlag2, setModalFlag2] = useState(false);
  const [pushBtnDisabled, setPushBtnDisabled] = useState(false);
  const [btnLoading, setBtnLoading] = useState(false);
  const { handleSubmit, register, getValues, errors, setValue, clearError } = useForm({ mode: "onBlur" });
  const formObj = { register, errors, setValue, clearError };

  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'ID', type: 'string', field: 'jobId', sorting: false, cellStyle: {fontFamily: 'Lucida Console'}},
    { title: 'Inference Name', type: 'string', field: 'jobName', sorting: false},
    { title: 'Type', type: 'string', field: 'modelconversionType', sorting: false },
    { title: 'Time', type: 'datetime', field: 'jobTime', 
      render: renderDate(getDate), customSort: sortDate(getDate) },
    { title: 'Status', type: 'string', field: 'modelconversionStatus' },
  ], []);
  const options = useMemo<Options>(() => ({
    padding: 'dense',
    actionsColumnIndex: -1,
    pageSize
  }), [pageSize]);

  const getData = () => {
    const jobOwner = isAdmin ? 'all' : userName;
    axios.get(`/${selectedCluster}/teams/${selectedTeam}/ListModelConversionJob?jobOwner=${jobOwner}&limit=999`)
      .then((res: { data: any; }) => {
        const { data } = res;
        const temp1 = JSON.stringify(jobs.map((i: { jobStatus: any; }) => i.jobStatus));
        const temp2 = JSON.stringify(data?.map((i: { jobStatus: any; }) => i.jobStatus));
        if (temp1 !== temp2) setJobs(data);
      }, () => {
        message('error', `Failed to fetch inferences from cluster: ${selectedCluster}`);
      })
  }

  const getFdInfo = () => {
    let info = {};
    axios.get(`/${selectedCluster}/getFDInfo`)
    .then((res: { data: any; }) => {
      setFdInfo(res.data);
      info = res.data;
    })
    return info;
  }

  useEffect(() => {
    getData();
  }, [selectedCluster, selectedTeam]);

  useInterval(() => {
    getData();
  }, pollInterval);
  
  const onPush = async (item: any) => {
    const info = await getFdInfo();
    if (info) {
      axios.post(`/${selectedCluster}/pushModelToFD`, { jobId: item.jobId }).then((res: any) => {
        getData();
        message('success', `Push Inference successfully！`);
      })
    } else {
      message('error', `Please entee settings first`);
      setModalFlag2(true);
    }
  }

  const onCloseDialog = (type: number) => {
    type === 1 ? setModalFlag1(false) : setModalFlag2(false);
  }

  const onSubmitTransform = async (val: any) => {
    setBtnLoading(true);
    await axios.post(`/${selectedCluster}/teams/${selectedTeam}/postModelConversionJob`, {
      ...val,
      image: '',
      device: null,
      conversionType: type
    }).then((res: any) => {
      getData();
      message('success', `Edge Inference successfully！`);
      setModalFlag1(false);
    },  () => {
      message('error', `Edge Inference failed！`);
    })
    setBtnLoading(false);
  }

  const onSubmitSettings = async (val: any) => {
    setBtnLoading(true);
    await axios.post(`/${selectedCluster}/teams/${selectedTeam}/setFDInfo`, val).then((res: any) => {
      message('success', `Save Settings successfully！`);
      setModalFlag2(false);
    },  () => {
      message('error', `Save Settings failed！`);
    })
    setBtnLoading(false);
  }

  const getConvertionTypeOptions = () => {
    if (convertionTypes) {
      return convertionTypes.map((i: any) => <MenuItem value={i}>{i}</MenuItem>)
    }
    return null;
  }

  const openSettings = async () => {
    await axios.get(`/${selectedCluster}/getFDInfo`)
      .then((res: { data: any; }) => {
        setFdInfo(res.data);
      })
    setModalFlag2(true);
  }

  const openInference = () => {
    axios.get(`/${selectedCluster}/getModelConvertionTypes`)
      .then((res: { data: any; }) => {
        setConvertionTypes(res.data.conversionTypes);
      })
    setModalFlag1(true);
  }

  return (
    <div className="modelList">
      <div style={{ marginBottom: 20 }}>
        <Button variant="contained" color="primary" onClick={openInference}>
          New Inference
        </Button>
        <Button variant="contained" style={{ margin: '0 20px' }} color="primary" onClick={openSettings}>
          Settings
        </Button>
      </div>
      <MaterialTable
        title=""
        columns={columns}
        data={jobs}
        options={options}
        actions={[{
          icon: 'backup',
          tooltip: 'Push Model',
          onClick: onPush,
          disabled: pushBtnDisabled
        }]}
        onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
      />
      {modalFlag1 && 
      <Dialog open={modalFlag1} disableBackdropClick fullWidth>
        <DialogTitle>New Model Transform</DialogTitle>
        <form onSubmit={handleSubmit(onSubmitTransform)}>
          <DialogContent dividers>
            {/* <RadioGroup row aria-label="quiz" name="quiz" value={type} onChange={onTypeChange}>
              <FormControlLabel value="1" control={<Radio />} label="Caffe -> A310" />
              <FormControlLabel value="2" control={<Radio />} label="TensorFlow -> A310" />
            </RadioGroup> */}
            {/* <SelectTree label="Input Path" style={{ margin: '10px 0' }} formObj={formObj} name="in" />
            <SelectTree label="Output Path" style={{ margin: '10px 0' }} formObj={formObj} name="out" /> */}
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
              select
              label="Type"
              name="conversionType"
              fullWidth
              onChange={e => setType(e.target.value)}
              variant="filled"
              value={type}
              style={{ margin: '10px 0' }}
            >
              {getConvertionTypeOptions()}
            </TextField>
            <TextField
              label="Input Path"
              name="inputPath"
              fullWidth
              variant="filled"
              error={Boolean(errors. inputPath)}
              helperText={errors. inputPath ? errors. inputPath.message : ''}
              InputLabelProps={{ shrink: true }}
              style={{ margin: '10px 0' }}
              inputRef={register({ 
                required: 'Input Path is required！',
              })}
            />
            <TextField
              label="Output Path"
              name="outputPath"
              fullWidth
              variant="filled"
              error={Boolean(errors.outputPath)}
              helperText={errors.outputPath ? errors.outputPath.message : ''}
              InputLabelProps={{ shrink: true }}
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Output Path is required！',
              })}
            />
          </DialogContent>
          <DialogActions>
            <Button onClick={() => onCloseDialog(1)} color="primary" variant="outlined">Cancel</Button>
            <Button type="submit" color="primary" variant="contained" disabled={btnLoading} style={{ marginLeft: 8 }}>
              {btnLoading && <CircularProgress size={20}/>}Submit
            </Button>
          </DialogActions>
        </form>
      </Dialog>}
      {modalFlag2 && 
      <Dialog open={modalFlag2} disableBackdropClick fullWidth>
        <DialogTitle>Settings</DialogTitle>
        <form onSubmit={handleSubmit(onSubmitSettings)}>
          <DialogContent dividers>
            <TextField
              label="URL"
              name="url"
              fullWidth
              variant="filled"
              error={Boolean(errors.url)}
              defaultValue={fdInfo.url}
              helperText={errors.url ? errors.url.message : ''}
              InputLabelProps={{ shrink: true }}
              inputRef={register({
                required: 'URL is required！'
              })}
            />
            <TextField
              label="Username"
              name="username"
              fullWidth
              variant="filled"
              defaultValue={fdInfo.username}
              error={Boolean(errors.username)}
              helperText={errors.username ? errors.username.message : ''}
              InputLabelProps={{ shrink: true }}
              inputRef={register({
                required: 'Username is required！'
              })}
              style={{ margin: '20px 0' }}
            />
            <TextField
              label="Password"
              name="password"
              fullWidth
              variant="filled"
              defaultValue={fdInfo.password}
              error={Boolean(errors.password)}
              helperText={errors.password ? errors.password.message : ''}
              InputLabelProps={{ shrink: true }}
              inputRef={register({
                required: 'Password is required！'
              })}
            />
          </DialogContent>
          <DialogActions>
            <Button onClick={() => onCloseDialog(2)} color="primary" variant="outlined">Cancel</Button>
            <Button type="submit" color="primary" disabled={btnLoading} variant="contained" style={{ marginLeft: 8 }}>
              {btnLoading && <CircularProgress size={20}/>}Save
            </Button>
          </DialogActions>
        </form>
      </Dialog>}
    </div>
  ); 
};

export default Model;
