import React, { useState, FC, useEffect, useMemo } from "react";
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, RadioGroup, Radio, CircularProgress,
  FormControl, FormControlLabel, FormLabel, FormHelperText, TextField } from "@material-ui/core";
import _ from 'lodash';
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate } from '../JobsV2/tableUtils';
import './index.less';
import SelectTree from '../CommonComponents/SelectTree';
import { useForm } from "react-hook-form";

const Model: React.FC = () => {
  const getDate = (item: any) => new Date(item.time);
  const [pageSize, setPageSize] = useState(10);
  const [type, setType] = useState('1');
  const [typeHelperText, setTypeHelperText] = useState('');
  const [modalFlag1, setModalFlag1] = useState(false);
  const [modalFlag2, setModalFlag2] = useState(false);
  const [btnLoading, setBtnLoading] = useState(false);
  const [expandedNodeIds, setExpandedNodeIds] = useState<string[]>([]);
  const { handleSubmit, register, getValues, errors, setValue, clearError } = useForm({ mode: "onBlur" });
  const formObj = { register, errors, setValue, clearError };

  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'ID', type: 'string', field: 'ID', sorting: false },
    { title: 'Type', type: 'string', field: 'Type', sorting: false },
    { title: 'Time', type: 'datetime', field: 'time', 
      render: renderDate(getDate), customSort: sortDate(getDate) },
    { title: 'Status', type: 'string', field: 'Status' },
  ], []);
  const options = useMemo<Options>(() => ({
    padding: 'dense',
    actionsColumnIndex: -1,
    pageSize
  }), [pageSize]);
  
  const onPush = (item: any) => {

  }

  const test = [{
    ID: '34298573428957892543',
    Type: 'caff-A310',
    time: 'Fri, 12 Jun 2020 06:29:22 GMT',
    Status: 'success'
  }]

  const onCloseDialog = (type: number) => {
    type === 1 ? setModalFlag1(false) : setModalFlag2(false);
  }

 

  const onTypeChange = (event: React.ChangeEvent<{}>, value: string) => {
    setType(value);
  };

  const onSubmitTransform = (data: any) => {
    console.log('aaaaaa', data)
  }

  const onSubmitSettings = (data: any) => {
    console.log('getValues', data);
  }

  return (
    <div className="modelList">
      <MaterialTable
        title={
        <>
          <p>Model List</p>
          <Button variant="contained" color="primary" onClick={() => setModalFlag1(true)}>
            New Transform
          </Button>
          <Button variant="contained" style={{ margin: '0 20px' }} color="primary" onClick={() => setModalFlag2(true)}>
            Settings
          </Button>
        </>
        }
        columns={columns}
        data={test}
        options={options}
        actions={[{
          icon: 'backup',
          tooltip: 'Push Model',
          onClick: onPush
        }]}
        onChangeRowsPerPage={(pageSize: any) => setPageSize(pageSize)}
      />
      {modalFlag1 && 
      <Dialog open={modalFlag1} disableBackdropClick fullWidth>
        <DialogTitle>New Model Transform</DialogTitle>
        <form onSubmit={handleSubmit(onSubmitTransform)}>
          <DialogContent dividers>
            {/* <FormControl component="fieldset">
              
              <FormHelperText>{typeHelperText}</FormHelperText>
            </FormControl> */}
            <RadioGroup row aria-label="quiz" name="quiz" value={type} onChange={onTypeChange}>
              <FormControlLabel value="1" control={<Radio />} label="Caffe -> A310" />
              <FormControlLabel value="2" control={<Radio />} label="TensorFlow -> A310" />
            </RadioGroup>
            <SelectTree label="Input Path" style={{ margin: '10px 0' }} formObj={formObj} name="in" />
            <SelectTree label="Output Path" style={{ margin: '10px 0' }} formObj={formObj} name="out" />
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
              {btnLoading && <CircularProgress size={20}/>}Submit
            </Button>
          </DialogActions>
        </form>
      </Dialog>}
    </div>
  ); 
};

export default Model;
