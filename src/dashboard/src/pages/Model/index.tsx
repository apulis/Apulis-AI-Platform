import React, { useState, FC, useEffect, useMemo } from "react";
import { Button, Dialog, DialogTitle, DialogContent, DialogActions, RadioGroup, Radio, CircularProgress,
  FormControl, FormControlLabel, FormLabel, FormHelperText } from "@material-ui/core";
import _ from 'lodash';
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate } from '../JobsV2/tableUtils';
import './index.less';
import SelectTree from '../CommonComponents/SelectTree';

const Model: React.FC = () => {
  const getDate = (item: any) => new Date(item.time);
  const [pageSize, setPageSize] = useState(10);
  const [type, setType] = useState('');
  const [typeHelperText, setTypeHelperText] = useState('');
  const [modalFlag1, setModalFlag1] = useState(false);
  const [btnLoading, setBtnLoading] = useState(false);
  const [expandedNodeIds, setExpandedNodeIds] = useState<string[]>([]);

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

  const onCloseDialog1 = () => {
    setModalFlag1(false);
  }

  const onSubmitSettings = () => {

  }

  const onTypeChange = (event: React.ChangeEvent<{}>, value: string) => {
    setType(value);
  };

  const test111 = (e: React.MouseEvent<SVGSVGElement, MouseEvent>) => {
    e.preventDefault();
    console.log('11111')
  }

  const onNodeToggle = (e: any, nodeIds: any) => {
    e.stopPropagation();
    console.log('22',e)
    setExpandedNodeIds(nodeIds);
    console.log('nodeIdnodeId',nodeIds)
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
          <Button variant="contained" style={{ margin: '0 20px' }} color="primary">
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
      <Dialog open={modalFlag1} disableBackdropClick maxWidth='xs' fullWidth>
        <DialogTitle>New Model Transform</DialogTitle>
        <DialogContent dividers>
          <form>
            <FormControl component="fieldset">
              <RadioGroup row aria-label="quiz" name="quiz" value={type} onChange={onTypeChange}>
                <FormControlLabel value="1" control={<Radio />} label="Caffe -> A310" />
                <FormControlLabel value="2" control={<Radio />} label="TensorFlow -> A310" />
              </RadioGroup>
              <FormHelperText>{typeHelperText}</FormHelperText>
            </FormControl>
            <SelectTree label="Input Path" />
            <SelectTree label="Output Path" />
          </form>
        </DialogContent>
        <DialogActions>
          <Button onClick={onCloseDialog1} color="primary" variant="outlined">Cancel</Button>
          <Button onClick={onSubmitSettings} color="primary" variant="contained" disabled={btnLoading} style={{ marginLeft: 8 }}>
            {btnLoading && <CircularProgress size={20}/>}Submit
          </Button>
        </DialogActions>
      </Dialog>}
    </div>
  ); 
};

export default Model;
