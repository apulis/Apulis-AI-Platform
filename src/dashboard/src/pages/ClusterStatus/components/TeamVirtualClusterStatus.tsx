import React from "react";
import {
  Table,
  TableHead,
  TableRow,
  TableCell,
  TableBody,
  Radio,
  createMuiTheme,
  MuiThemeProvider,
  SvgIcon, Typography, CircularProgress, useTheme
} from "@material-ui/core";
import {checkObjIsEmpty, sumValues} from "../../../utlities/ObjUtlities";
import { TeamVCTitles } from "../../../Constants/TabsContants";
import useCheckIsDesktop from "../../../utlities/layoutUtlities";
import MaterialTable from "material-table";
import Tooltip from "@material-ui/core/Tooltip";
import IconButton from "@material-ui/core/IconButton";
import {red} from "@material-ui/core/colors";

interface TeamVC {
  children?: React.ReactNode;
  vcStatus: any;
  selectedValue: string;
  handleChange: any;
}

const tableTheme = createMuiTheme({
  overrides: {
    MuiTableCell: {
      root: {
        paddingTop: 4,
        paddingBottom: 4,
        paddingLeft:2,
        paddingRight:4,
      }
    }
  }
});

const renderData = (data: any) => {
  return (
    <span>
      {
        !data ? 0 :
          checkObjIsEmpty(Object.values(data))  ? 0 : (Number)(sumValues(data))
      }
    </span>
  )
}
export const TeamVirtualClusterStatus = (props: TeamVC) => {
  const {vcStatus,selectedValue,handleChange, children} = props;
  const hasLen = vcStatus.length > 0 && vcStatus[0];
  const theme = useTheme();
  const gpuCapacity = hasLen ? vcStatus[0].gpu_capacity : {};
  const gpuUnschedulable = hasLen ? vcStatus[0].gpu_unschedulable : {};
  const gpuUsed = hasLen ? vcStatus[0].gpu_used : {};
  const gpuPreemptableUsed = hasLen ? vcStatus[0].gpu_preemptable_used : {};
  const gpuAvaliable = hasLen ? vcStatus[0].gpu_avaliable : {};
  const gpuAvaliableJobNum = hasLen ? vcStatus[0].AvaliableJobNum : {}
  const gpuStatusList: any = Object.keys(gpuCapacity).map(val => {
    return {
      type: val,
      total: gpuCapacity[val] || 0,
      unschedulable: gpuUnschedulable[val] || 0,
      used: gpuUsed[val] || 0,
      preemptibleUsed: gpuPreemptableUsed[val] || 0,
      avaliable: gpuAvaliable[val] || 0,
      avaliableJobNum: gpuAvaliableJobNum[val] || 0,
      clusterName: hasLen ? vcStatus[0].ClusterName : '',
    }
  })
  return (
    <MuiThemeProvider theme={useCheckIsDesktop ? theme : tableTheme}>
      {
        gpuStatusList.length > 0 ? 
        <MaterialTable
          title=""
          columns={[
            {title: 'Name', field: 'clusterName', render: (rowData: any)=><div>{rowData['clusterName']}</div>, customSort:(a, b) => a['clusterName'].localeCompare(b['clusterName'])},
            {title: 'Device Type', field: 'type'},
            {title: 'Number of Device', field: 'total'},
            {title: 'Unschedulable', field: 'unschedulable'},
            {title: 'Used', field: 'used'},
            {title: 'Preemptible Used', field: 'preemptibleUsed'},
            {title: 'Available', field: 'avaliable'},
            {title: 'Active Jobs', field: 'avaliableJobNum'}
          ]}
          data={gpuStatusList}
          options={{filtering: false, paging: true, pageSize: gpuStatusList.length < 10 ? gpuStatusList.length : 10, pageSizeOptions: [10], sorting: true}}
        /> :
          <CircularProgress/>
      }
    </MuiThemeProvider>

  )
}



// <MaterialTable
//   title=""
//   columns={[
    // {title: 'Name', field: 'ClusterName', render:(rowData: any)=><div><Radio
    //   checked={selectedValue === rowData['ClusterName']}
    //   onChange={handleChange}
    //   value={rowData['ClusterName']}
    //   name={rowData['ClusterName']}/>{rowData['ClusterName']}</div>, customSort:(a, b) => a['ClusterName'].localeCompare(b['ClusterName'])},
//     {title: 'Total GPU', field: '', render:(rowData: any)=>renderData(rowData['gpu_capacity']),customSort:(a, b) => sumValues(a['gpu_capacity']) - sumValues(b['gpu_capacity'])},
    // {title: 'Unschedulable GPU', field: '', render:(rowData: any)=>renderData(rowData['gpu_unschedulable']), customSort:(a, b) => sumValues(a['gpu_unschedulable']) - sumValues(b['gpu_unschedulable'])},
    // {title: 'Used GPU', field: '', render:(rowData: any)=>renderData(rowData['gpu_used']),customSort: (a, b) => sumValues(a['gpu_used']) - sumValues(b['gpu_used'])},
    // {title: 'Preemptible Used GPU', field: '', render:(rowData: any)=>renderData(rowData['gpu_preemptable_used']), customSort: (a, b) => !a || !b ? -1 : sumValues(a['gpu_preemptable_used']) - sumValues(b['gpu_preemptable_used'])},
    // {title: 'Available GPU', field: '', render:(rowData: any)=>renderData(rowData['gpu_avaliable']),customSort: (a, b) => sumValues(a['gpu_avaliable']) - sumValues(b['gpu_avaliable'])},
    // {title: 'Active Jobs', field: '', render:(rowData: any)=><span>{ Number(sumValues(rowData['AvaliableJobNum'])) || 0}</span>, customSort: (a, b) => sumValues(a['AvaliableJobNum']) - sumValues(b['AvaliableJobNum'])}
//   ]}
//   data={gpuStatusList}
//   options={{filtering: false,paging: true, pageSize: vcStatus.length < 10 ? vcStatus.length  : 10 ,pageSizeOptions:[10],sorting: true}}
// />