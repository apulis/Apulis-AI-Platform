import React, { useState, FC, useEffect, useMemo, useContext } from "react";
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, CircularProgress, TextField, 
  MenuItem } from "@material-ui/core";
import _ from 'lodash';
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate } from '../JobsV2/tableUtils';
// import './index.less';
import SelectTree from '../CommonComponents/SelectTree';
import { useForm } from "react-hook-form";
import ClusterContext from '../../contexts/Clusters';
import useActions from "../../hooks/useActions";
import { NameReg, NameErrorText } from '../../const';

const CentralReasoning: React.FC = () => {
  const { selectedCluster, availbleGpu } = useContext(ClusterContext);
  const [pageSize, setPageSize] = useState(10);
  const [modalFlag, setModalFlag] = useState(false);
  const [btnLoading, setBtnLoading] = useState(false);
  const { handleSubmit, register, getValues, errors, setValue, clearError } = useForm({ mode: "onBlur" });
  const { kill } = useActions(selectedCluster);
  const actions = [kill];
  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'ID', type: 'string', field: 'ID', sorting: false, cellStyle: {fontFamily: 'Lucida Console'}},
    { title: 'Jobname', type: 'string', field: 'Jobname'},
    { title: 'Username', type: 'string', field: 'Username'},
    { title: 'Path', type: 'string', field: 'Path', sorting: false },
    { title: 'Framework', type: 'string', field: 'Framework', sorting: false },
    { title: 'DeviceType', type: 'string', field: 'DeviceType', sorting: false },
    { title: 'Status', type: 'string', field: 'jobStatus'},
    { title: 'URL', type: 'string', field: 'URL', sorting: false }
  ], []);
  const options = useMemo<Options>(() => ({
    padding: 'dense',
    actionsColumnIndex: -1,
    pageSize
  }), [pageSize]);

  const test = [{
    ID: '34298573428957892543',
    Type: 'caff-A310',
    jobStatus: 'running'
  }]
 
  const onSubmit = (data: any) => {
    console.log('aaaaaa', data)
  }

  return (
    <div className="modelList">
      <MaterialTable
        title={
          <Button variant="contained" color="primary" onClick={() => setModalFlag(true)}>
            New Reasoning
          </Button>
        }
        columns={columns}
        data={test}
        options={options}
        actions={actions}
        onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
      />
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
              name="path"
              fullWidth
              variant="filled"
              error={Boolean(errors.path)}
              helperText={errors.path ? errors.path.message : ''}
              InputLabelProps={{ shrink: true }}
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Path is required！',
              })}
            />
            <TextField
              select
              label="Device Type"
              name="type"
              fullWidth
              variant="filled"
              defaultValue="CPU"
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Device Type is required！'
              })}
            >
              <MenuItem value="CPU">CPU</MenuItem>
            </TextField>
            <TextField
              select
              label="Framework"
              name="framework"
              fullWidth
              variant="filled"
              style={{ margin: '10px 0' }}
              inputRef={register({
                required: 'Framework is required！'
              })}
            >
              <MenuItem value="CPU">CPU</MenuItem>
            </TextField>
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
