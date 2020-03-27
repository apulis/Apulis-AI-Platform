import React from "react"
import {
  Table,
  TableHead,
  TableRow, TableCell, TableBody, Button, Grid, FormControl, InputLabel, Select, MenuItem, TextField,
  Container
} from "@material-ui/core";
import axios from 'axios';
import ClustersContext from "../../contexts/Clusters";
import { Autorenew } from "@material-ui/icons";

export default class Access extends React.Component {
  static contextType = ClustersContext
  constructor() {
    super()
    this.state = {
      vcList: [],
      accessList: [],
      modifyFlag: false,
      isEdit: 0,
      identityName: '',
      resourceType: null,
      resourceName: null,
      permissions: null,
    }
  }

  componentWillMount() {
    this.getAccessList();
    this.getVcList();
  }

  getAccessList = () => {
    axios.get(`/${this.context.selectedCluster}/GetACL`)
      .then((res) => {
        this.setState({
          accessList: res.data.result,
        })
      }, () => { })
  }
  getVcList = () => {
    axios.get(`/${this.context.selectedCluster}/listVc`)
      .then((res) => {
        this.setState({
          vcList: res.data.result,
        })
      }, () => { })
  }


  addAccess = () => {
    const { modifyFlag } = this.state;
    this.setState({
      modifyFlag: !modifyFlag,
      isEdit: 0,
      identityName: '',
      resourceType: 1,
      resourceName: '',
      permissions: 1,
    })
  }

  updateAccess = (item) => {
    let resourceType, resourceName;
    if (item.resource === 'Cluster') {
      resourceType = 1;
    } else {
      resourceType = 2;
      resourceName = item.resource.split(':')[1];
    }
    const { modifyFlag } = this.state;
    this.setState({
      modifyFlag: !modifyFlag,
      identityName: item.identityName,
      resourceType,
      resourceName,
      permissions: item.permissions,
      isEdit: 1,
    })
  }

  save = () => {
    const { resourceType, resourceName, permissions, identityName } = this.state;
    let url = `/${this.context.selectedCluster}/updateAce?resourceType=${resourceType}&resourceName=${resourceName}&permissions=${permissions}&identityName=${identityName}`;
    axios.get(url).then((res) => {
      alert(`操作成功`)
      this.getAccessList();
      this.setState({ modifyFlag: false })
    }, (e) => {
      console.log(e);
      alert(`操作失败`)
    })
  }

  delete = (item) => {
    let resourceType, resourceName;
    if (window.confirm('确认删除')) {
      if (item.resource === 'Cluster') {
        resourceType = 1;
      } else {
        resourceType = 2;
        resourceName = item.resource.split(':')[1];
      }
      let url = `/${this.context.selectedCluster}/deleteAce?resourceType=${resourceType}&resourceName=${resourceName}&identityName=${item.identityName}`;
      axios.get(url)
        .then((res) => {
          this.getAccessList();
        }, () => { })
    }
  }


  //change
  resourceTypeChange(e) {
    this.setState({
      resourceType: e.target.value
    })
  }

  resourceNameChange(e) {
    this.setState({
      resourceName: e.target.value
    })
  }

  permissionsChange(e) {
    this.setState({
      permissions: e.target.value
    })
  }

  identityNameChange(e) {
    this.setState({
      identityName: e.target.value
    })
  }

  render() {
    const { accessList, modifyFlag, isEdit, identityName, resourceType, resourceName, permissions, vcList } = this.state;
    const PermMap = {
      0: 'Unauthorized',
      1: 'User',
      3: 'Collaborator',
      7: 'Admin'
    }
    return (
      <Container fix maxWidth="xl">
        <div>
          <div style={{marginLeft: 'auto', marginRight: 'auto'}}>
            <Button variant="outlined" size="medium" color="primary" onClick={this.addAccess}>{modifyFlag && !isEdit ? '收起新增' : '新增ACCESS'}</Button>
          </div>
          <Table style={{ width: '85%', marginTop: 20}}>
            <TableHead>
              <TableRow style={{ backgroundColor: '#7583d1' }}>
                <TableCell style={{ color: '#fff' }}>identityId</TableCell>
                <TableCell style={{ color: '#fff' }}>identityName</TableCell>
                <TableCell style={{ color: '#fff' }}>isDeny</TableCell>
                <TableCell style={{ color: '#fff' }}>permissions</TableCell>
                <TableCell style={{ color: '#fff' }}>resource</TableCell>
                <TableCell style={{ color: '#fff' }}>actions</TableCell>
              </TableRow>
            </TableHead>
            <TableBody>
              {accessList.map(item => (
                <TableRow key={item.id}>
                  <TableCell>{item.identityId} </TableCell>
                  <TableCell>{item.identityName} </TableCell>
                  <TableCell>{item.isDeny} </TableCell>
                  <TableCell>{PermMap[item.permissions]} </TableCell>
                  <TableCell>{item.resource}</TableCell>
                  <TableCell>
                    <Button color="primary" onClick={() => this.updateAccess(item)}>Modify</Button>
                    <Button color="primary" onClick={() => this.delete(item)}>Delete</Button>
                  </TableCell>
                </TableRow>
              ))}
            </TableBody>
          </Table>
          {
            modifyFlag ?
              <div style={{ width: '25%', float: 'left', padding: 10, margin: 10, borderWidth: 2, borderColor: '#999', borderStyle: 'solid' }}>
                <h2 id="simple-modal-title">{isEdit ? '编辑' : '新增'}</h2>
                {!isEdit ? <p>理论上不用新增，用户第三方登录后就会有ACL记录</p> : null}
                <form>

                  {
                    isEdit === 1 ?
                      <div>
                        <Grid item xs={8} style={{ marginTop: 20 }}>
                          {identityName}
                        </Grid>
                        <Grid item xs={8} style={{ marginTop: 20 }}>
                          {resourceType === 1 ? 'Cluster' : 'Cluster/VC:' + resourceName}
                        </Grid>
                      </div>
                      :
                      <div>
                        <Grid item xs={8} style={{ marginTop: 20 }}>
                          <TextField
                            required
                            label="identityName"
                            value={identityName}
                            onChange={this.identityNameChange.bind(this)}
                            margin="normal"
                            fullWidth={true}
                          />
                        </Grid>
                        <Grid item xs={8} style={{ marginTop: 20 }}>
                          <FormControl>
                            <InputLabel>resourceType</InputLabel>
                            <Select
                              value={resourceType}
                              onChange={this.resourceTypeChange.bind(this)}
                            >
                              <MenuItem value={1}>Cluster</MenuItem>
                              <MenuItem value={2}>VC</MenuItem>
                            </Select>
                          </FormControl>
                        </Grid>
                        {
                          resourceType === 2 ?
                            <Grid item xs={8} style={{ marginTop: 20 }}>
                              <FormControl>
                                <InputLabel>resourceName</InputLabel>
                                <Select
                                  value={resourceName}
                                  onChange={this.resourceNameChange.bind(this)}
                                >
                                  {vcList.map(item => (
                                    <MenuItem key={item.vcName} value={item.vcName}>{item.vcName}</MenuItem>
                                  ))}
                                </Select>
                              </FormControl>
                            </Grid> : null
                        }
                      </div>
                  }
                  <Grid item xs={8} style={{ marginTop: 20 }}>
                    <FormControl>
                      <InputLabel>permissions</InputLabel>
                      <Select
                        value={permissions}
                        onChange={this.permissionsChange.bind(this)}
                      >
                        <MenuItem value={0}>Unauthorized</MenuItem>
                        <MenuItem value={1}>User</MenuItem>
                        <MenuItem value={3}>Collaborator</MenuItem>
                        <MenuItem value={7}>Admin</MenuItem>
                      </Select>
                    </FormControl>
                  </Grid>
                  <Grid item xs={8} style={{ marginTop: 10 }}>
                    <Button variant="outlined" size="medium" color="primary" type="button" onClick={this.save}>Save</Button>
                  </Grid>
                </form>
              </div>
              : null
          }
        </div>
      </Container>
    )
  }
}