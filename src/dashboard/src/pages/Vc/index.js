import React from "react"
import {
  Table, TableHead, TableRow, TableCell, TableBody, 
  Button, TextField, Grid, Container,
  Dialog, DialogActions, DialogContent, DialogTitle, DialogContentText,
  CircularProgress, MenuItem
} from "@material-ui/core";
import axios from 'axios';
import ClustersContext from "../../contexts/Clusters";
import message from '../../utils/message';
import { NameReg, NameErrorText, SameNameErrorText } from '../../const';
import './index.less';
import _ from 'lodash';

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
      btnLoading: false,
      allDevice: {},
      qSelectData: {},
      mSelectData: {}
    }
  }

  componentDidMount() {
    const { selectedCluster, userName } = this.context;
    this.getVcList();
    axios.get(`/${selectedCluster}/getAllDevice?userName=${userName}`)
      .then((res) => {
        console.log('allDevice',res.data)
        const allDevice = res.data;
        let qSelectData = {}, mSelectData = {};
        Object.keys(allDevice).forEach(i => {
          qSelectData[i] = null;
          mSelectData[i] = null;
        });
        this.setState({ allDevice, qSelectData, mSelectData });
      })
  }

  getVcList = () => {
    axios.get(`/${this.context.selectedCluster}/listVc`)
      .then((res) => {
        console.log('vcList',res.data.result)
        this.setState({
          vcList: res.data.result,
        })
      })
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
    const { vcName, quota, metadata } = item;
    const { selectedCluster, userName } = this.context;
    axios.get(`/${selectedCluster}/countJobByStatus?userName=${userName}&targetStatus=running,scheduling,killing,pausing&vcName=${vcName}`)
    .then((res) => {
      if (res.data > 0) {
        message('warning','No running, scheduling, killing, or pausing job is required to perform operations!');
        return
      } else {
        const { modifyFlag } = this.state;
        this.setState({
          modifyFlag: true,
          isEdit: 1,
          vcName: vcName,
          qSelectData: JSON.parse(quota),
          mSelectData: JSON.parse(metadata).user_quota
        })
      }
    })
  }

  save = async () => {
    const { isEdit, vcName, vcNameValidateObj, qSelectData, mSelectData } = this.state;
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
    let url, quota = _.cloneDeep(qSelectData), metadata = _.cloneDeep(mSelectData), canSave = true;
    Object.keys(quota).forEach(i => {
      if (quota[i] === null) quota[i] = 0;
      if (metadata[i] === null) metadata[i] = 0;
      if (metadata[i] > quota[i]) {
        message('error', 'The value of metadata cannot be greater than the value of quota！');
        canSave = false;
        return;
      }
    });
    if (!canSave) return;
    quota = JSON.stringify(quota);
    metadata = JSON.stringify({user_quota: metadata});
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
    const { selectedCluster, userName } = this.context;
    const { vcName } = this.state.deleteItem;
    axios.get(`/${selectedCluster}/countJobByStatus?userName=${userName}&targetStatus=running,scheduling,killing,pausing&vcName=${vcName}`)
    .then((res) => {
      if (res.data > 0) {
        message('warning','No running, scheduling, killing, or pausing job is required to perform operations!');
        return
      } else {
        axios.get(`/${selectedCluster}/deleteVc/${vcName}`)
        .then((res) => {
          message('success', 'Delete successfully！');
          this.getVcList();
        }, () => { 
          message('error', 'Delete failed！');
        })
      }
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

  getSelectHtml = (type) => {
    const { allDevice, qSelectData, mSelectData, vcList, isEdit } = this.state;
    return Object.keys(allDevice).map(m => {
      let num = allDevice[m].capacity, val = null, options = {}, oldVal = {};
      type === 1 ? val = qSelectData[m] : val = mSelectData[m];
      type === 1 ? oldVal = qSelectData : oldVal = mSelectData;
      vcList.forEach(n => {
        const useNum = JSON.parse(n.quota)[m];
        num = useNum ? num - useNum : num;
      })
      options[m] = Number(num);
      const key = type === 1 ? 'qSelectData' : 'mSelectData';
      return (
        <div className="select-item">
          <TextField
            label="Type"
            variant="outlined"
            value={m}
            disabled
            className="select-key"
          ></TextField>
          <TextField
            select
            label="Value"
            variant="outlined"
            className="select-value"
            value={isEdit ? val : null}
            onChange={e => this.setState({ [key]: { ...oldVal, [m]: e.target.value } })}
          >
            {this.getOptions(options[m], val)}
          </TextField>
        </div>
      )
    })
  }

  getOptions = (data, val) => {
    let content = [];
    const _data = val !== null && val > data ? val : data; 
    for(let i = 0; i <= _data; i++){
      content.push(<MenuItem key={i} value={i}>{i}</MenuItem>)
    }
    return content;
  }

  render() {
    const { vcList, modifyFlag, isEdit, vcName, quota, metadata, vcNameValidateObj, deleteModifyFlag, deleteItem, btnLoading, qSelectData, mSelectData } = this.state;
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
          <Dialog open={modifyFlag} disableBackdropClick maxWidth='xs' fullWidth onClose={() => this.setState({modifyFlag: false})}>
            <DialogTitle>{isEdit ? 'Modify' : 'ADD'}</DialogTitle>
            <DialogContent dividers>
              <form>
                <TextField
                  required
                  label="vcName"
                  value={vcName}
                  onChange={this.vcNameChange}
                  margin="normal"
                  error={vcNameValidateObj.error}
                  fullWidth={true}
                  disabled={isEdit}
                  helperText={vcNameValidateObj.text}
                />
                <h3>quota</h3>
                {this.getSelectHtml(1)}
                <h3>metadata/user_quota</h3>
                {this.getSelectHtml(2)}
              </form>
            </DialogContent>
            <DialogActions>
              <Button onClick={() => this.setState({modifyFlag: false})} color="primary" variant="outlined">Cancel</Button>
              <Button onClick={this.save} color="primary" variant="contained" disabled={btnLoading}>
                {btnLoading && <CircularProgress size={20}/>}Save
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
                  {btnLoading && <CircularProgress size={20}/>}Delete
                </Button>
              </DialogActions>
            </Dialog>
          }
        </div>
      </Container>
      
    )
  }
}