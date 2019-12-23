import React from "react"
import {
  Table,
  TableHead,
  TableRow, TableCell, TableBody, Button, Grid, FormControl, InputLabel, Select, MenuItem,
} from "@material-ui/core";
import axios from 'axios';
import ClustersContext from "../../contexts/Clusters";

export default class User extends React.Component {
  static contextType = ClustersContext
  constructor() {
    super()
    this.state = {
      userList: [],
      modifyFlag: false,
      userName: '',
      isAdmin: null,
      isAuthorized: null,
    }
  }

  componentWillMount() {
    this.getUserList();
  }

  getUserList = () => {
    axios.get(`/api/${this.context.selectedCluster}/listUser`)
      .then((res) => {
        this.setState({
          userList: res.data,
        })
      }, () => { })
  }


  updateUser = (item) => {
    const { modifyFlag } = this.state;
    this.setState({
      modifyFlag: !modifyFlag,
      userName: item.userName,
      isAdmin: item.isAdmin,
      isAuthorized: item.isAuthorized,
    })
  }

  save = () => {
    const { isAdmin, isAuthorized, userName } = this.state;
    let url = `/api/${this.context.selectedCluster}/updateUserPerm/${isAdmin}/${isAuthorized}/${userName}`;
    axios.get(url).then(() => {
      alert(`修改成功`)
      this.getUserList();
    }, (e) => {
      console.log(e);
      alert(`修改失败`)
    })
  }

  //change
  isAdminChange(e) {
    this.setState({
      isAdmin: e.target.value
    })
  }

  isAuthorizedChange(e) {
    this.setState({
      isAuthorized: e.target.value
    })
  }

  render() {
    const { userList, modifyFlag, userName, isAdmin, isAuthorized } = this.state;
    return (
      <div>
        <Table style={{ float: 'left', width: '70%' }}>
          <TableHead>
            <TableRow style={{ backgroundColor: '#7583d1' }}>
              <TableCell style={{ color: '#fff' }}>uid</TableCell>
              <TableCell style={{ color: '#fff' }}>openId</TableCell>
              <TableCell style={{ color: '#fff' }}>group</TableCell>
              <TableCell style={{ color: '#fff' }}>nickName</TableCell>
              <TableCell style={{ color: '#fff' }}>userName</TableCell>
              <TableCell style={{ color: '#fff' }}>isAdmin</TableCell>
              <TableCell style={{ color: '#fff' }}>isAuthorized</TableCell>
              <TableCell style={{ color: '#fff' }}>actions</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {userList.map(item => (
              <TableRow key={item.uid}>
                <TableCell>{item.uid} </TableCell>
                <TableCell>{item.openId} </TableCell>
                <TableCell>{item.group} </TableCell>
                <TableCell>{item.nickName} </TableCell>
                <TableCell>{item.userName}</TableCell>
                <TableCell>{item.isAdmin ? '管理员' : '用户'} </TableCell>
                <TableCell>{item.isAuthorized ? '有权限' : '没权限'} </TableCell>
                <TableCell>
                  <Button color="primary" onClick={() => this.updateUser(item)}>Modify</Button>
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
        {
          modifyFlag ?
            <div style={{ float: 'left', width: '25%', padding: 10, margin: 10, borderWidth: 2, borderColor: '#999', borderStyle: 'solid' }}>
              <h2 id="simple-modal-title">修改</h2>
              <form>
                <Grid item xs={8}>
                  {userName}
                </Grid>
                <Grid item xs={8} style={{ marginTop: 20 }}>
                  <FormControl>
                    <InputLabel>isAdmin</InputLabel>
                    <Select
                      value={isAdmin}
                      onChange={this.isAdminChange.bind(this)}
                    >
                      <MenuItem value={1}>True</MenuItem>
                      <MenuItem value={0}>false</MenuItem>
                    </Select>
                  </FormControl>
                  <FormControl style={{ marginLeft: 20 }}>
                    <InputLabel>isAuthorized</InputLabel>
                    <Select
                      value={isAuthorized}
                      onChange={this.isAuthorizedChange.bind(this)}
                    >
                      <MenuItem value={1}>True</MenuItem>
                      <MenuItem value={0}>false</MenuItem>
                    </Select>
                  </FormControl>
                </Grid>
                <Grid item xs={8} style={{ marginTop: 20 }}>
                  <Button variant="outlined" size="medium" color="primary" type="button" onClick={this.save}>Save</Button>
                </Grid>
              </form>
            </div>
            : null
        }
      </div>
    )
  }
}