import React from "react"
import {
  Table,
  TableHead,
  TableRow, TableCell, TableBody, Button, Grid, FormControl, InputLabel, Select, MenuItem,
} from "@material-ui/core";
import axios from 'axios';

export default class User extends React.Component {
  constructor() {
    super()
    this.state = {
      userList: [],
      modifyFlag: false,
      identityName: '',
      isAdmin: null,
      isAuthorized: null,
    }
  }

  componentWillMount() {
    this.getUserList();
  }

  getUserList = () => {
    axios.get('/api/qjydev/listUser')
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
      identityName: item.identityName,
      isAdmin: item.isAdmin,
      isAuthorized: item.isAuthorized,
    })
  }

  save = () => {
    const { isAdmin, isAuthorized, identityName } = this.state;
    let url = `/api/qjydev/updateUserPerm/${isAdmin}/${isAuthorized}/${identityName}`;
    axios.get(url).then((res) => {
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
    const { userList, modifyFlag, identityName, isAdmin, isAuthorized } = this.state;
    return (
      <div>
        {
          modifyFlag ?
            <div style={{ width: '50%', padding: 10, margin: 10, borderWidth: 2, borderColor: '#999', borderStyle: 'solid' }}>
              <h2 id="simple-modal-title">修改</h2>
              <form>
                <Grid item xs={8}>
                  {identityName}
                </Grid>
                <Grid item xs={8}>
                  <FormControl>
                    <InputLabel id="demo-simple-select-label">isAdmin</InputLabel>
                    <Select
                      value={isAdmin}
                      onChange={this.isAdminChange.bind(this)}
                    >
                      <MenuItem value={1}>True</MenuItem>
                      <MenuItem value={0}>false</MenuItem>
                    </Select>
                  </FormControl>
                  <FormControl>
                    <InputLabel id="demo-simple-select-label">isAuthorized</InputLabel>
                    <Select
                      value={isAuthorized}
                      onChange={this.isAuthorizedChange.bind(this)}
                    >
                      <MenuItem value={1}>True</MenuItem>
                      <MenuItem value={0}>false</MenuItem>
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
        <Table>
          <TableHead>
            <TableRow>
              <TableCell>uid</TableCell>
              <TableCell>identityName</TableCell>
              <TableCell>Alias</TableCell>
              <TableCell>Password</TableCell>
              <TableCell>groups</TableCell>
              <TableCell>isAdmin</TableCell>
              <TableCell>isAuthorized</TableCell>
              <TableCell>actions</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {userList.map(item => (
              <TableRow key={item.uid}>
                <TableCell>{item.uid} </TableCell>
                <TableCell>{item.identityName} </TableCell>
                <TableCell>{item.Alias} </TableCell>
                <TableCell>{item.Password} </TableCell>
                <TableCell>{item.groups}/{item.gid}</TableCell>
                <TableCell>{item.isAdmin ? '管理员' : '用户'} </TableCell>
                <TableCell>{item.isAuthorized ? '有权限' : '没权限'} </TableCell>
                <TableCell>
                  <Button color="primary" onClick={() => this.updateUser(item)}>Modify</Button>
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </div>
    )
  }
}