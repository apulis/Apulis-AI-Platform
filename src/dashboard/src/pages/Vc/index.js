import React from "react"
import {
  Table,
  TableHead,
  TableRow, TableCell, TableBody, Button, TextField, Grid,
} from "@material-ui/core";
import axios from 'axios';
import ClustersContext from "../../contexts/Clusters";

export default class Vc extends React.Component {
  static contextType = ClustersContext
  constructor() {
    super()
    this.state = {
      vcList: [],
      modifyFlag: false,
      isEdit: 0,
      vcName: '',
      quota: '',
      metadata: '',
    }
  }

  componentWillMount() {
    this.getVcList();
  }

  getVcList = () => {
    axios.get(`/api/${this.context.selectedCluster}/listVc`)
      .then((res) => {
        this.setState({
          vcList: res.data.result,
        })
      }, () => { })
  }

  //新增VC
  addVc = () => {
    const { modifyFlag } = this.state;
    this.setState({
      modifyFlag: !modifyFlag,
      isEdit: 0,
      vcName: '',
      quota: '',
      metadata: '',
    })
  }

  updateVc = (item) => {
    const { modifyFlag } = this.state;
    this.setState({
      modifyFlag: !modifyFlag,
      isEdit: 1,
      vcName: item.vcName,
      quota: item.quota,
      metadata: item.metadata,
    })
  }

  save = () => {
    const { isEdit, vcName, quota, metadata } = this.state;
    if (!this.isJSON(quota)) {
      alert('quota必须是json格式');
      return;
    }
    if (!this.isJSON(metadata)) {
      alert('metadata必须是json格式');
      return;
    }
    let url;
    if (isEdit) {
      url = `/api/${this.context.selectedCluster}/updateVc/${vcName}/${quota}/${metadata}`;
    } else {
      url = `/api/${this.context.selectedCluster}/addVc/${vcName}/${quota}/${metadata}`;
    }
    axios.get(url)
      .then((res) => {
        alert(`${isEdit ? '修改' : '新增'}成功`)
      }, (e) => {
        console.log(e);
        alert(`${isEdit ? '修改' : '新增'}失败`)
      })
  }

  delete = (item) => {
    const { vcList } = this.state;
    if (vcList.length === 1) {
      alert('必须保留一个vc');
      return;
    }
    if (window.confirm('确认删除')) {
      axios.get(`/api/${this.context.selectedCluster}/deleteVc/${item.vcName}`)
        .then((res) => {
          this.getVcList();
        }, () => { })
    }
    // 删除逻辑todo: 关联的表记录删除
  }

  //change
  vcNameChange(e) {
    this.setState({
      vcName: e.target.value
    })
  }

  quotaChange(e) {
    this.setState({
      quota: e.target.value
    })
  }

  metadataChange(e) {
    this.setState({
      metadata: e.target.value
    })
  }

  isJSON(str) {
    if (typeof str == 'string') {
      try {
        var obj = JSON.parse(str);
        if (typeof obj == 'object' && obj) {
          return true;
        } else {
          return false;
        }
      } catch (e) {
        return false;
      }
    }
  }

  render() {
    const { vcList, modifyFlag, isEdit, vcName, quota, metadata } = this.state;
    return (
      <div>
        <div>
          <Button variant="outlined" size="medium" color="primary" onClick={this.addVc}>{modifyFlag && !isEdit ? '收起新增' : '新增VC'}</Button>
        </div>
        <Table style={{ width: '70%', float: 'left', marginTop: 20 }}>
          <TableHead>
            <TableRow style={{ backgroundColor: '#7583d1' }}>
              <TableCell style={{ color: '#fff' }}>vcName</TableCell>
              <TableCell style={{ color: '#fff' }}>quota</TableCell>
              <TableCell style={{ color: '#fff' }}>metadata</TableCell>
              <TableCell style={{ color: '#fff' }}>admin</TableCell>
              <TableCell style={{ color: '#fff' }}>actions</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {vcList.map(item => (
              <TableRow key={item.vcName}>
                <TableCell>{item.vcName} </TableCell>
                <TableCell>{item.quota} </TableCell>
                <TableCell>{item.metadata} </TableCell>
                <TableCell>{item.admin ? '管理员' : '用户'} </TableCell>
                <TableCell>
                  <Button color="primary" onClick={() => this.updateVc(item)}>Modify</Button>
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
              <form>
                <Grid item xs={8}>
                  <TextField
                    required
                    label="vcName"
                    value={vcName}
                    onChange={this.vcNameChange.bind(this)}
                    margin="normal"
                    fullWidth={true}
                  />
                </Grid>
                <Grid item xs={8}>
                  <TextField
                    required
                    label="quota"
                    value={quota}
                    onChange={this.quotaChange.bind(this)}
                    margin="normal"
                    fullWidth={true}
                  />
                </Grid>
                <Grid item xs={8}>
                  <TextField
                    required
                    label="metadata"
                    value={metadata}
                    onChange={this.metadataChange.bind(this)}
                    margin="normal"
                    fullWidth={true}
                  />
                </Grid>
                <Grid item xs={8}>
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