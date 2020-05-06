import React from "react"
import {
  Table, TableHead, TableRow, TableCell, TableBody, 
  Button, TextField, Grid, Container,
  Dialog, DialogActions, DialogContent, DialogTitle, DialogContentText,
  CircularProgress
} from "@material-ui/core";
import axios from 'axios';
import ClustersContext from "../../contexts/Clusters";
import message from '../../utils/message';
import { NameReg, NameErrorText, SameNameErrorText } from '../../const';

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
      vcNameValidateObj: {
        text: '',
        error: false
      },
      deleteModifyFlag: false,
      deleteItem: {},
      btnLoading: false
    }
  }

  componentWillMount() {
    this.getVcList();
  }

  getVcList = () => {
    axios.get(`/${this.context.selectedCluster}/listVc`)
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
      modifyFlag: true,
      isEdit: 1,
      vcName: item.vcName,
      quota: item.quota,
      metadata: item.metadata,
    })
  }

  save = async () => {
    const { isEdit, vcName, quota, metadata, vcNameValidateObj } = this.state;
    const { selectedCluster } = this.context;
    if (!vcName || vcNameValidateObj.error) {
      this.setState({
        vcNameValidateObj: {
          error: true,
          text: vcNameValidateObj.text || 'vcName is required！'
        }
      })
      return;
    };
    if (!this.isJSON(quota)) {
      alert('quota必须是json格式');
      return;
    }
    if (!this.isJSON(metadata)) {
      alert('metadata必须是json格式');
      return;
    }
    let url;
    this.setState({ btnLoading: true });
    if (isEdit) {
      url = `/${selectedCluster}/updateVc/${vcName}/${quota}/${metadata}`;
    } else {
      url = `/${selectedCluster}/addVc/${vcName}/${quota}/${metadata}`;
    }
    await axios.get(url)
      .then((res) => {
        message('success', `${isEdit ? 'Modified' : 'Added'}  successfully！`);
        this.setState({modifyFlag: false });
        this.getVcList();
      }, (e) => {
        message('error', `${isEdit ? 'Modified' : 'Added'}  failed！`);
      })
    this.setState({ btnLoading: false });
  }

  delete = () => {
    axios.get(`/${this.context.selectedCluster}/deleteVc/${this.state.deleteItem.vcName}`)
      .then((res) => {
        message('success', 'Delete successfully！');
        this.getVcList();
      }, () => { 
        message('error', 'Delete failed！');
      })
  }

  vcNameChange = e => {
    const { vcList } = this.state;
    const val = e.target.value;
    const hasNames = vcList.map(i => i.vcName);
    const error = !val || !NameReg.test(val) || hasNames.includes(val) ? true : false;
    const text = !val ? 'vcName is required！' : !NameReg.test(val) ? NameErrorText : hasNames.includes(val) ? SameNameErrorText : '';
    this.setState({ 
      vcName: val,
      vcNameValidateObj: {
        error: error,
        text: text
      }
    })
  }

  quotaChange = e => {
    this.setState({
      quota: e.target.value
    })
  }

  metadataChange = e => {
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
    const { vcList, modifyFlag, isEdit, vcName, quota, metadata, vcNameValidateObj, deleteModifyFlag, deleteItem, btnLoading } = this.state;
    return (
      <Container fixed maxWidth="xl">
        <div style={{marginLeft: 'auto', marginRight: 'auto'}}>
          <div><Button variant="outlined" size="medium" color="primary" onClick={this.addVc}>ADD</Button></div>
          <Table style={{ width: '80%', marginTop: 20 }}>
            <TableHead>
              <TableRow style={{ backgroundColor: '#7583d1' }}>
                <TableCell style={{ color: '#fff' }}>vcName</TableCell>
                <TableCell style={{ color: '#fff' }}>quota</TableCell>
                <TableCell style={{ color: '#fff' }}>metadata</TableCell>
                <TableCell style={{ color: '#fff' }}>permissions</TableCell>
                <TableCell style={{ color: '#fff' }}>actions</TableCell>
              </TableRow>
            </TableHead>
            <TableBody>
              {vcList.map(item => (
                <TableRow key={item.vcName}>
                  <TableCell>{item.vcName} </TableCell>
                  <TableCell>{item.quota} </TableCell>
                  <TableCell>{item.metadata} </TableCell>
                  <TableCell>{item.admin ? 'Admin' : 'User'} </TableCell>
                  <TableCell>
                    <Button color="primary" onClick={() => this.updateVc(item)}>Modify</Button>
                    <Button color="secondary" disabled={item.vcName === this.context.selectedTeam} 
                      onClick={() => this.setState({ deleteModifyFlag: true, deleteItem: item })}>Delete</Button>
                  </TableCell>
                </TableRow>
              ))}
            </TableBody>
          </Table>
          {modifyFlag && 
          <Dialog open={modifyFlag} maxWidth='xs' fullWidth onClose={() => this.setState({modifyFlag: false})}>
            <DialogTitle>{isEdit ? 'Modify' : 'ADD'}</DialogTitle>
            <DialogContent dividers>
              <form>
                <Grid item xs={8}> 
                  <TextField
                    label="vcName"
                    value={vcName}
                    onChange={this.vcNameChange}
                    margin="normal"
                    error={vcNameValidateObj.error}
                    fullWidth={true}
                    disabled={isEdit}
                    helperText={vcNameValidateObj.text}
                  />
                </Grid>
                <Grid item xs={8}>
                  <TextField
                    required
                    label="quota"
                    value={quota}
                    onChange={this.quotaChange}
                    margin="normal"
                    fullWidth={true}
                  />
                </Grid>
                <Grid item xs={8}>
                  <TextField
                    required
                    label="metadata"
                    value={metadata}
                    onChange={this.metadataChange}
                    margin="normal"
                    fullWidth={true}
                  />
                </Grid>
              </form>
            </DialogContent>
            <DialogActions>
              <Button onClick={() => this.setState({modifyFlag: false})} color="primary" variant="outlined">Cancel</Button>
              <Button onClick={this.save} color="primary" variant="contained" disabled={btnLoading}>
                {btnLoading && <CircularProgress size={20}/>}
                Save
              </Button>
            </DialogActions>
          </Dialog>}
          {deleteModifyFlag && 
            <Dialog open={deleteModifyFlag} maxWidth='xs' fullWidth onClose={() => this.setState({ deleteModifyFlag: false })}>
              <DialogTitle>Delete</DialogTitle>
              <DialogContent>
                <DialogContentText>Are you sure to delete this VC？</DialogContentText>
              </DialogContent>
              <DialogActions>
                <Button onClick={() => this.setState({ deleteModifyFlag: false })} color="primary" variant="outlined">Cancel</Button>
                <Button onClick={this.delete} color="secondary" variant="contained" disabled={btnLoading}>
                  {btnLoading && <CircularProgress size={20}/>}
                  Delete
                </Button>
              </DialogActions>
            </Dialog>
          }
        </div>
      </Container>
      
    )
  }
}