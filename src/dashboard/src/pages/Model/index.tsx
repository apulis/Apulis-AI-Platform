import React, { useState, FC, useEffect, useMemo } from "react";
import { Button } from "@material-ui/core";
import _ from 'lodash';
import MaterialTable, { Column, Options } from 'material-table';
import { renderDate, sortDate } from '../JobsV2/tableUtils';
import './index.less';
import TransformIcon from '@material-ui/icons/Transform';
import CloudUploadIcon from '@material-ui/icons/CloudUpload';

const Model: React.FC = () => {
  const getDate = (item: any) => new Date(item.time);
  const [pageSize, setPageSize] = useState(10);
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

  return (
    <div className="modelList">
      <MaterialTable
        title={
        <>
          Model List
          <Button variant="contained" style={{ marginLeft: 20 }} color="primary">
            New Transform
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
        onChangeRowsPerPage={pageSize => setPageSize(pageSize)}
      />
    </div>
  );
};

export default Model;
