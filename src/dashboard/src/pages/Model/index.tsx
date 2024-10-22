import React, { useState, FC, useEffect, useMemo, useContext, useCallback } from "react";
import {
  Button, Dialog, DialogTitle, DialogContent, DialogActions, RadioGroup, Radio, CircularProgress,
  FormControlLabel, TextField, MenuItem
} from "@material-ui/core";
import MaterialTable, { Column, Options, Action } from 'material-table';
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
import { useTranslation } from "react-i18next";

const Model: React.FC = () => {
  const { t } = useTranslation();
  const inferenceNameTip = t('Inference Name is required！');
  const inputPathTip = t('Input Path is required！');
  const outputPathTip = t('Output Path is required！');
  const URLTip = t('URL is required！');
  const usernameTip = t('Username is required！');
  const passwordTip = t('Password is required！');
  const { currentRole = [], userName } = useContext(AuthContext);
  const getDate = (item: any) => new Date(item.jobTime);
  // const isAdmin = permissionList.includes('EDGE_INFERENCE');
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
  const [pushId, setPushId] = useState('');
  const [btnLoading, setBtnLoading] = useState(false);
  const { handleSubmit, register, getValues, errors, setValue, clearError } = useForm({ mode: "onBlur" });
  const formObj = { register, errors, setValue, clearError };
  const renderStatus = (job: any) => {
    const { jobStatus, modelconversionStatus } = job;
    let status = modelconversionStatus;
    if (modelconversionStatus === 'converting') {
      status = jobStatus === 'finished' ? t('convert success') : jobStatus === 'failed' ? t('convert failed') : modelconversionStatus;
    }
    return <p>{t(status)}</p>
  }
  const options = useMemo<Options>(() => ({
    padding: 'dense',
    actionsColumnIndex: -1,
    pageSize
  }), [pageSize]);

  const getData = () => {
    // const jobOwner = isAdmin ? 'all' : userName;
    const jobOwner = userName;
    axios.get(`/${selectedCluster}/teams/${selectedTeam}/ListModelConversionJob?jobOwner=${jobOwner}&limit=999`)
      .then((res: { data: any }) => {
        const { data } = res;
        const temp1 = JSON.stringify(jobs.map((i: { jobStatus: any }) => i.jobStatus));
        const temp2 = JSON.stringify(data?.map((i: { jobStatus: any }) => i.jobStatus));
        const temp3 = JSON.stringify(jobs.map((i: { modelconversionStatus: any }) => i.modelconversionStatus));
        const temp4 = JSON.stringify(data?.map((i: { modelconversionStatus: any }) => i.modelconversionStatus));
        if (temp1 !== temp2 || temp3 !== temp4) setJobs(data);
      }, () => {  
        message('error', `${t('Failed to fetch inferences from cluster')}: ${selectedCluster}`);
      })
  }

  const getFdInfo = () => {
    let info = {};
    axios.get(`/${selectedCluster}/getFDInfo`)
      .then((res: { data: any }) => {
        setFdInfo(res.data);
        info = res.data;
      })
    return info;
  }

  useEffect(() => {
    getFdInfo();
    getData();
  }, [selectedCluster, selectedTeam]);

  useInterval(() => {
    getData();
  }, pollInterval);

  const onPush = async (e: any, item: any) => {
    const info = await getFdInfo();
    const successTip = t('Submitted successfully！');
    const errorTip = t('Please enter settings first');
    if (info) {
      const { jobId } = item;
      setPushId(jobId);
      axios.post(`/${selectedCluster}/pushModelToFD`, { jobId: jobId }).then((res: any) => {
        getData();
        message('success', successTip);
      }).catch(err => {
        console.log(err);
        setPushId('');
      })
    } else {
      message('error', errorTip);
      setModalFlag2(true);
    }
  }

  const push = useCallback((job: any): Action<any> => {
    const { jobStatus, modelconversionStatus, jobId } = job;
    const pushInference = t('Push Inference');
    const disabled = (!(modelconversionStatus === 'converting' && jobStatus === 'finished') || pushId === jobId);
    return {
      icon: 'backup',
      tooltip: pushInference,
      onClick: onPush,
      disabled: disabled
    }
  }, [onPush]);

  const onCloseDialog = (type: number) => {
    type === 1 ? setModalFlag1(false) : setModalFlag2(false);
  }

  const onSubmitTransform = async (val: any) => {
    setBtnLoading(true);
    const successTip = t('Submitted successfully！');
    const errorTip = t('Submitted failed！');
    await axios.post(`/${selectedCluster}/teams/${selectedTeam}/postModelConversionJob`, {
      ...val,
      image: '',
      device: null,
      conversionType: type
    }).then((res: any) => {
      getData();
      message('success', successTip);
      setModalFlag1(false);
    }, () => {
      message('error', errorTip);
    })
    setBtnLoading(false);
  }

  const onSubmitSettings = async (val: any) => {
    setBtnLoading(true);
    const successTip = t('Save Settings successfully！');
    const errorTip = t('Save Settings failed！');
    await axios.post(`/${selectedCluster}/teams/${selectedTeam}/setFDInfo`, val).then((res: any) => {
      message('success', successTip);
      setModalFlag2(false);
    }, () => {
      message('error', errorTip);
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
    await getFdInfo();
    setModalFlag2(true);
  }

  const openInference = () => {
    axios.get(`/${selectedCluster}/getModelConvertionTypes`)
      .then((res: { data: any }) => {
        const { conversionTypes } = res.data;
        setConvertionTypes(conversionTypes);
        setType(conversionTypes[1]);
      })
    setModalFlag1(true);
  }

  return (
    <div className="modelList">
      <div style={{ marginBottom: 20 }}>
        <Button variant="contained" color="primary" onClick={openInference}>
          {t("New Inference")}
        </Button>
        <Button variant="contained" style={{ margin: '0 20px' }} color="primary" onClick={openSettings}>
          {t("Settings")}
        </Button>
        {fdInfo.url !== '' && <Button variant="contained" color="primary" href={fdInfo.url} target="blank">
          {t("FD Server")}
        </Button>}
      </div>
      <MaterialTable
        title=""
        columns={[
          { title: 'ID', type: 'string', field: 'jobId', sorting: false, cellStyle: { fontFamily: 'Lucida Console' } },
          { title: t('Inference Name'), type: 'string', field: 'jobName', sorting: false },
          { title: t('Type'), type: 'string', field: 'modelconversionType', sorting: false },
          {
            title: t('Time'), type: 'datetime', field: 'jobTime',
            render: renderDate(getDate), customSort: sortDate(getDate)
          },
          { title: t('Status'), type: 'string', field: 'modelconversionStatus', render: renderStatus },
        ]}
        data={jobs}
        options={options}
        actions={[push]}
        onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
        localization={{
          pagination: {
            labelDisplayedRows: '{from}-{to} of {count}',
            firstTooltip: t('jobsV2.firstPage'),
            previousTooltip: t('jobsV2.previousPage'),
            nextTooltip: t('jobsV2.nextPage'),
            lastTooltip: t('jobsV2.lastPage'),
            labelRowsPerPage: t('jobsV2.labelRowsPerPage'),
            labelRowsSelect: t('jobsV2.labelRowsSelect')
          },
          toolbar: {
            nRowsSelected: '{0} row(s) selected',
            searchTooltip: t('jobsV2.search'),
            searchPlaceholder: t('jobsV2.search')
          },
          header: {
            actions: t('jobsV2.actions')
          },
          body: {
            emptyDataSourceMessage: t('jobsV2.noRecordsToDisplay'),
            filterRow: {
              filterTooltip: 'Filter'
            }
          }
        }}
      />
      {modalFlag1 &&
        <Dialog open={modalFlag1} disableBackdropClick fullWidth>
          <DialogTitle>{t("New Inference")}</DialogTitle>
          <form onSubmit={handleSubmit(onSubmitTransform)}>
            <DialogContent dividers>
              {/* <RadioGroup row aria-label="quiz" name="quiz" value={type} onChange={onTypeChange}>
              <FormControlLabel value="1" control={<Radio />} label="Caffe -> A310" />
              <FormControlLabel value="2" control={<Radio />} label="TensorFlow -> A310" />
            </RadioGroup> */}
              {/* <SelectTree label="Input Path" style={{ margin: '10px 0' }} formObj={formObj} name="in" />
            <SelectTree label="Output Path" style={{ margin: '10px 0' }} formObj={formObj} name="out" /> */}
              <TextField
                label={t("Inference Name")}
                name="jobName"
                fullWidth
                variant="filled"
                error={Boolean(errors.jobName)}
                helperText={errors.jobName ? errors.jobName.message : ''}
                InputLabelProps={{ shrink: true }}
                inputProps={{ maxLength: 20 }}
                style={{ margin: '10px 0' }}
                inputRef={register({
                  required: inferenceNameTip,
                  pattern: {
                    value: NameReg,
                    message: NameErrorText
                  }
                })}
              />
              <TextField
                select
                label={t("Type")}
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
                label={t("Input Path")}
                name="inputPath"
                fullWidth
                variant="filled"
                error={Boolean(errors.inputPath)}
                helperText={errors.inputPath ? errors.inputPath.message : ''}
                InputLabelProps={{ shrink: true }}
                style={{ margin: '10px 0' }}
                inputRef={register({
                  required: inputPathTip,
                })}
              />
              <TextField
                label={t("Output Path")}
                name="outputPath"
                fullWidth
                variant="filled"
                error={Boolean(errors.outputPath)}
                helperText={errors.outputPath ? errors.outputPath.message : ''}
                InputLabelProps={{ shrink: true }}
                style={{ margin: '10px 0' }}
                inputRef={register({
                  required: outputPathTip,
                })}
              />
            </DialogContent>
            <DialogActions>
              <Button onClick={() => onCloseDialog(1)} color="primary" variant="outlined">{t('Cancel')}</Button>
              <Button type="submit" color="primary" variant="contained" disabled={btnLoading} style={{ marginLeft: 8 }}>
                {btnLoading && <CircularProgress size={20} />}{t('Submit')}
              </Button>
            </DialogActions>
          </form>
        </Dialog>}
      {modalFlag2 &&
        <Dialog open={modalFlag2} disableBackdropClick fullWidth>
          <DialogTitle>{t("Settings")}</DialogTitle>
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
                  required: URLTip
                })}
              />
              <TextField
                label={t("Username")}
                name="username"
                fullWidth
                variant="filled"
                defaultValue={fdInfo.username}
                error={Boolean(errors.username)}
                helperText={errors.username ? errors.username.message : ''}
                InputLabelProps={{ shrink: true }}
                inputRef={register({
                  required: usernameTip
                })}
                style={{ margin: '20px 0' }}
              />
              <TextField
                label={t("Password")}
                name="password"
                fullWidth
                variant="filled"
                defaultValue={fdInfo.password}
                error={Boolean(errors.password)}
                helperText={errors.password ? errors.password.message : ''}
                InputLabelProps={{ shrink: true }}
                inputRef={register({
                  required: passwordTip
                })}
              />
            </DialogContent>
            <DialogActions>
              <Button onClick={() => onCloseDialog(2)} color="primary" variant="outlined">{t('Cancel')}</Button>
              <Button type="submit" color="primary" disabled={btnLoading} variant="contained" style={{ marginLeft: 8 }}>
              {btnLoading && <CircularProgress size={20} />}{t("Save")}
              </Button>
            </DialogActions>
          </form>
        </Dialog>}
    </div>
  );
};

export default Model;
