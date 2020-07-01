import React from "react";
import {
  Tooltip,
  Switch,
  CircularProgress
} from "@material-ui/core";
import MaterialTable, {MTableToolbar} from "material-table";
import UserContext from '../../../contexts/User';
interface TeamUsr {
  userStatus: any;
  showCurrentUser: boolean;
  handleSwitch: any;
  currentCluster: string;
}

export const TeamVCUserStatus = (props: TeamUsr) => {
  const { userStatus, showCurrentUser, handleSwitch, currentCluster } = props;
  const { userName } = React.useContext(UserContext);

  if (currentCluster === 'Lab-RR1-V100') {
    return (
      <>
        {
          userStatus ?  <MaterialTable
            title=""
            columns={[{title: 'Username', field: 'userName'},
              {title: 'Currently Allocated GPU', field: 'usedGPU',type:'numeric'},
              {title: 'Currently Allocated Preemptible GPU', field: 'preemptableGPU',type:'numeric'},
              {title: 'Currently Idle GPU', field: 'idleGPU',type:'numeric'},
            ]}
            data={showCurrentUser ? userStatus.filter((uc: any)=>uc['usedGPU'] > 0 && uc['userName'] !== 'Total') : userStatus}
            options={{filtering: false,paging: false,sorting: false}}
            components={{
              Toolbar: props => (
                <div>
                  <MTableToolbar {...props} />
                  <Tooltip title="Show Current User">
                    <Switch
                      checked={showCurrentUser}
                      onChange={handleSwitch}
                    />
                  </Tooltip>
                </div>
              )
            }}
          /> :
            <CircularProgress/>
        }
      </>
    )
  }
  return (
    <>
      {
        userStatus ?  <MaterialTable
          title=""
          columns={[{title: 'Username', field: 'userName'},
            {title: 'Device Type', field: 'gpuType',type:'numeric'},
            {title: 'Currently Allocated', field: 'usedGPU',type:'numeric'},
            {title: 'Currently Allocated Preemptible', field: 'preemptableGPU',type:'numeric', render: (rowData: any) => <span>{rowData['preemptableGPU'] ? rowData['preemptableGPU'] : '0'}</span>},
            {title: 'Currently Idle', field: 'idleGPU',type:'numeric'},
            {title: 'Past Month Booked Hour', field: 'booked',type:'numeric', render: (rowData: any) => <span>{rowData['booked'] ? rowData['booked'] : '0'}</span>},
            {title: 'Past Month Idle Hour', field: 'idle',type:'numeric', render: (rowData: any) => <span>{rowData['idle'] ? rowData['idle'] : '0'}</span>},
            {title: 'Past Month Idle Hour %', field: '',type:'numeric', render: (rowData: any) => currentCluster === 'Lab-RR1-V100' ? null : <span style={{ color: Math.floor((rowData['idle'] / rowData['booked']) * 100) > 50 ? "red" : "black" }}>{rowData['booked'] == '0' ? '-' : Math.floor(((rowData['idle'] || 0) / (rowData['booked'])) * 100)}</span>, customSort: (a: any, b: any) => {return Math.floor((a['idle'] / a['booked']) * 100) - Math.floor((b['idle'] / b['booked']) * 100)}}
          ]}
          data={showCurrentUser ? userStatus.filter((uc: any)=>uc['usedGPU'] > 0 && uc['userName'] !== 'Total') : userStatus}
          options={{filtering: false, paging: false, sorting: false}}
          components={{
            Toolbar: props => (
              <div>
                <MTableToolbar {...props} />
                <Tooltip title={showCurrentUser ? "Show All Use" : "Show Current Use"}>
                  <Switch
                    checked={showCurrentUser}
                    onChange={handleSwitch}
                  />
                </Tooltip>
              </div>
            )
          }}
        /> :
          <CircularProgress/>
      }
    </>
  )
}
