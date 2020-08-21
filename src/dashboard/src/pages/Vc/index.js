import React from "react"
import {
  Table, TableHead, TableRow, TableCell, TableBody, 
  Button, TextField, Container,
  Dialog, DialogActions, DialogContent, DialogTitle, DialogContentText,
  CircularProgress, MenuItem, Select, Tooltip
} from "@material-ui/core";
import axios from 'axios';
import ClustersContext from "../../contexts/Clusters";
import message from '../../utils/message';
import { NameReg, NameErrorText, SameNameErrorText } from '../../const';
import './index.less';
import _ from 'lodash';
import AuthzHOC from '../../components/AuthzHOC';
import { Pagination } from '@material-ui/lab';
import Loading from '../../components/Loading';
import { Help } from "@material-ui/icons"; 

const empty = {
  text: '',
  error: false
}

export default class Vc extends React.Component {
  static contextType = ClustersContext
  constructor() {
    super()
    this.state = {
      vcList: [],
      modifyFlag: false,
      isEdit: 0, 
      vcName: '',
      vcNameValidateObj: empty,
      quotaValidateObj: {},
      metadataValidateObj: {},
      deleteModifyFlag: false,
      btnLoading: false,
      allDevice: {},
      qSelectData: {},
      mSelectData: {},
      clickItem: {},
      page: 1,
      size: 10,
      count: 0,
      loading: false
    }
  }

  componentDidMount() {
    const { selectedCluster } = this.context;
    this.getVcList();
    axios.get(`/${selectedCluster}/getAllDevice`)
      .then((res) => {
        const allDevice = res.data;
        let qSelectData = {}, mSelectData = {}, quotaValidateObj = {}, metadataValidateObj = {};
        Object.keys(allDevice).forEach(i => {
          qSelectData[i] = null;
          mSelectData[i] = null;
          quotaValidateObj[i] = empty;
          metadataValidateObj[i] = empty;
        });
        this.setState({ allDevice, qSelectData, mSelectData, quotaValidateObj, metadataValidateObj });
      })
  }

  getVcList = () => {
    this.setState({ loading: true });
    const { page, size } = this.state;
    axios.get(`/${this.context.selectedCluster}/listVc?page=${page}&size=${size}`)
      .then((res) => {
        const { totalNum, result } = res.data;
        if (!result.length && totalNum) {
          this.setState({ page: 1 }, () => {
            this.getVcList();
          });
        } else {
          this.setState({
            vcList: result,
            count: Math.ceil(totalNum / size),
            loading: false
          })
        }
      })
  }

  //新增VC
  addVc = () => {
    const { modifyFlag } = this.state;
    this.setState({
      modifyFlag: !modifyFlag,
      isEdit: 0,
      vcName: ''
    })
  }

  updateVc = (item) => {
    const { vcName, quota, metadata } = item;
    this.checkCountJobByStatus(vcName, () => {
      const { modifyFlag } = this.state;
      const qSelectData = JSON.parse(quota);
      const _mSelectData = JSON.parse(metadata);
      this.setState({
        modifyFlag: true,
        isEdit: 1,
        vcName: vcName,
        qSelectData,
        mSelectData: Object.keys(_mSelectData).length > 0 ? _mSelectData : {},
        clickItem: item
      })
    });
  }

  save = async () => {
    const { isEdit, vcName, vcNameValidateObj, qSelectData, mSelectData, allDevice, quotaValidateObj, metadataValidateObj } = this.state;
    const { selectedCluster, getTeams } = this.context;
    let flag = true;
    if (!vcName || vcNameValidateObj.error) {
      this.setState({
        vcNameValidateObj: {
          error: true,
          text: vcNameValidateObj.text ? vcNameValidateObj.text : 'VCName is required！'
        }
      })
      return;
    }
    Object.keys(quotaValidateObj).forEach(i => {
      if (quotaValidateObj[i].error) flag = false;
    })
    Object.keys(metadataValidateObj).forEach(i => {
      if (metadataValidateObj[i].error) flag = false;
    })
    if (!flag) return;
    let url, quota = {}, metadata = {}, canSave = true;
    if (Object.keys(allDevice).length > 0) {
      quota = _.cloneDeep(qSelectData);
      metadata = _.cloneDeep(mSelectData);
      Object.keys(quota).forEach(i => {
        if (!allDevice[i]) { 
          delete quota[i];
          delete metadata[i];
        }
        if (quota[i] === null) quota[i] = 0;
        if (metadata[i] === null || !metadata[i]) metadata[i] = {user_quota: 0};
        if (metadata[i].user_quota > quota[i]) {
          this.setState({
            metadataValidateObj: {
              ...metadataValidateObj,
              [i]: {
                error: true,
                text: 'Cannot be greater than DeviceNumber'
              }
            }
          })
          canSave = false;
          return;
        }
      });
    }
    if (!canSave) return;
    quota = JSON.stringify(quota);
    metadata = JSON.stringify(metadata);
    this.setState({ btnLoading: true });
    url = `/${selectedCluster}/${isEdit ? 'updateVc' : 'addVc'}/${vcName}/${quota}/${metadata}`;
    await axios.get(url)
      .then((res) => {
        message('success', `${isEdit ? 'Modified' : 'Added'}  successfully！`);
        this.onCloseDialog();
        getTeams();
        this.getVcList();
      }, (e) => {
        message('error', `${isEdit ? 'Modified' : 'Added'}  failed！`);
      })
    this.setState({ btnLoading: false });
  }

  delete = () => {
    const { selectedCluster, getTeams } = this.context;
    const { vcName } = this.state.delItem;
    this.setState({ btnLoading: true });
    axios.get(`/${selectedCluster}/deleteVc/${vcName}`)
      .then((res) => {
        if (res.data.result === false) {
          message('error', 'Could not delete last VC');
          return;
        }
        message('success', 'Delete successfully！');
        this.setState({ deleteModifyFlag: false, btnLoading: false });
        getTeams();
        this.getVcList();
      }, () => { 
        message('error', 'Delete failed！');
        this.setState({ btnLoading: false });
      })
  }

  vcNameChange = e => {
    const { vcList } = this.state;
    const val = e.target.value;
    const hasNames = vcList.map(i => i.vcName);
    const error = !val || !NameReg.test(val) || hasNames.includes(val) ? true : false;
    const text = !val ? 'VCName is required！' : !NameReg.test(val) ? NameErrorText : hasNames.includes(val) ? SameNameErrorText : '';
    this.setState({ 
      vcName: val,
      vcNameValidateObj: {
        error: error,
        text: text
      }
    })
  }

  getSelectHtml = (type) => {
    const { allDevice, qSelectData, mSelectData, vcList, isEdit, clickItem, quotaValidateObj, metadataValidateObj } = this.state;
    return Object.keys(allDevice).map(m => {
      const totalNum = allDevice[m].capacity;
      let val = null, options = {}, oldVal = {}, num = allDevice[m].capacity;
      if (type == 1) {
        val = qSelectData[m];
        oldVal = qSelectData;
      } else {
        val =  mSelectData[m] && mSelectData[m].user_quota !== null ? mSelectData[m].user_quota : null;
        oldVal = mSelectData;
      }
      vcList.forEach(n => {
        const useNum = JSON.parse(n.quota)[m];
        num = useNum ? (num - useNum < 0 ? 0 : num - useNum) : num;
      })
      options[m] = Number(num);
      const editData = isEdit ? JSON.parse(clickItem[type === 1 ? 'quota' : 'metadata'])[m] : null;
      const temp = editData && editData.constructor  === Object ? editData.user_quota : editData;
      let key, _error, _helperText, maxData;
      if (!isEdit && options[m] === 0) val = 0;
      if (type === 1) {
        key = 'qSelectData';
        _error = quotaValidateObj[m] ? quotaValidateObj[m].error : false;
        _helperText = quotaValidateObj[m] ? quotaValidateObj[m].text : '';
        maxData = temp ? editData + num : options[m];
      } else {
        key = 'mSelectData';
        _error = metadataValidateObj[m] ? metadataValidateObj[m].error : false;
        _helperText = metadataValidateObj[m] ? metadataValidateObj[m].text : '';
        maxData = this.state.qSelectData[m]
      }
      return (
        <div className="select-item">
          <TextField
            label="Type"
            variant="outlined"
            value={m}
            disabled
            className="select-key"
          />
          <TextField
            type="number"
            label="Value"
            variant="outlined"
            className="select-value"
            defaultValue={val || 0}
            onChange={e => this.onNumValChange(key, oldVal, m, type, e.target.value, maxData)}
            inputProps={{min: "0", max: maxData, step: "1"}}
            error={_error}
            helperText={_helperText}
          />
        </div>
      )
    })
  }

  onNumValChange = (key, oldVal, m, type, val, max) => {
    const _val = Number(val);
    const { quotaValidateObj, metadataValidateObj } = this.state;
    const stateKey = type === 1 ? 'quotaValidateObj' : 'metadataValidateObj';
    let stateVal = {};
    if (!Number.isInteger(_val) || _val > max || _val < 0) {
      const obj = {
        error: true,
        text: _val > max ? `Cannot be greater than DeviceNumber` : `Must be a positive integer from 0 to ${max}`
      };
      stateVal = type === 1 ? { ...quotaValidateObj, [m]: obj } : { ...metadataValidateObj, [m]: obj };
      this.setState({
        [stateKey]: stateVal
      });
      return;
    } else {
      stateVal = type === 1 ? { ...quotaValidateObj, [m]: empty } : { ...metadataValidateObj, [m]: empty };
      this.setState({
        [stateKey]: stateVal
      });
    }
    this.setState({
      [key]: { ...oldVal, [m]: type === 1 ? _val : { user_quota: _val }}
    })
  }

  onClickDel = item => {
    const { vcName } = item;
    this.checkCountJobByStatus(vcName, () => {
      this.setState({ deleteModifyFlag: true, delItem: item })
    });
  }

  checkCountJobByStatus = (vcName, callback) => {
    const { selectedCluster } = this.context;
    const targetStatus = encodeURIComponent('running,scheduling,killing,pausing');
    axios.get(`/${selectedCluster}/countJobByStatus/${targetStatus}/${vcName}`)
    .then((res) => {
      if (res.data > 0) {
        message('warning','No running, scheduling, killing, or pausing job is required to perform operations!');
        return
      } else {
        callback();
      }
    })
  }

  onCloseDialog = () => {
    let qSelectData = this.state.qSelectData;
    Object.keys(qSelectData).forEach(i => qSelectData[i] = 0);
    Object.keys(mSelectData).forEach(i => mSelectData[i]['user_quota'] = 0);
    this.setState({
      modifyFlag: false,
      vcNameValidateObj: empty,
      quotaValidateObj: {},
      metadataValidateObj: {},
      qSelectData,
    })
  }

  onPageChange = (e, page) => {
    this.setState({ page }, () => {
      this.getVcList();
    });
  }

  onSizeChange = (e) => {
    this.setState({
      size: Number(e.target.value),
      page: 1
    }, () => {
      this.getVcList();
    });
  }

  getDeviceTypeContent = (v, isNum, isMetadata) => {
    const val = JSON.parse(v);
    const keys = Object.keys(val);
    let content = null;
    if (keys.length) {
      isNum ? content = keys.map(i => <p>{isMetadata ? val[i].user_quota : val[i]}</p>)  : content = keys.map(i => <p>{i}</p>)
    }
    return content;
  }

  render() {
    const { vcList, modifyFlag, isEdit, vcName, deleteModifyFlag, btnLoading, qSelectData, mSelectData, 
      allDevice, vcNameValidateObj, page, count, size, loading } = this.state;

    if (loading) return <Loading/>

    return (
      <Container fixed maxWidth="xl">
        <div className="vcWrap">
          <AuthzHOC needPermission={'MANAGE_VC'}>
            <Button variant="outlined" size="medium" color="primary" onClick={this.addVc}>ADD</Button>
          </AuthzHOC>
          <Table>
            <TableHead> 
              <TableRow>
                <TableCell>VCName</TableCell>
                <TableCell>DeviceType</TableCell>
                <TableCell>DeviceNumber</TableCell>
                <TableCell>
                  <Tooltip title="Maximum number of devices available to each user">
                    <div className="helpWrap">MaxAvailable<Help fontSize="small" /></div>
                  </Tooltip>
                </TableCell>
                <TableCell>
                  <Tooltip title="The number of users who own this VC resource">
                    <div className="helpWrap">UserNumber<Help fontSize="small" /></div>
                  </Tooltip>
                </TableCell>
                <TableCell>Users</TableCell>
                <AuthzHOC needPermission={'MANAGE_VC'}><TableCell>Actions</TableCell></AuthzHOC>
              </TableRow>
            </TableHead>
            <TableBody>
              {vcList.map(item => (
                <TableRow key={item.vcName}>
                  <TableCell>{item.vcName} </TableCell>
                  <TableCell>{this.getDeviceTypeContent(item.quota)}</TableCell>
                  <TableCell>{this.getDeviceTypeContent(item.quota, true)}</TableCell>
                  <TableCell>{this.getDeviceTypeContent(item.metadata, true, true)}</TableCell>
                  <TableCell>{item.userNum}</TableCell>
                  <TableCell style={{ maxWidth: 250 }}><div className="textEllipsis" title={item.userNameList || '--'}>{item.userNameList || '--'}</div></TableCell>
                  <AuthzHOC needPermission={'MANAGE_VC'}>
                    <TableCell>
                      <Button color="primary" onClick={() => this.updateVc(item)}>Modify</Button>
                      <Button color="secondary" disabled={item.vcName === this.context.selectedTeam} 
                        onClick={() => this.onClickDel(item)}>Delete</Button>
                    </TableCell>
                  </AuthzHOC>
                </TableRow>
              ))}
            </TableBody>
          </Table>
          <div className="paginationWrap">
            <Select
                value={size}
                onChange={this.onSizeChange}
                displayEmpty
              >
                {[10,20,30,40,50].map(i => <MenuItem value={i}>{i} rows</MenuItem>)}
            </Select>
            <Pagination
              color="primary"
              count={count}
              page={page}
              style={{ display: 'inline-block' }}
              onChange={this.onPageChange}
            />
          </div>
          {modifyFlag && 
          <Dialog open={modifyFlag} disableBackdropClick maxWidth='xs' fullWidth>
            <DialogTitle>{isEdit ? 'Modify' : 'ADD'}</DialogTitle>
            <DialogContent dividers>
              <form>
                <TextField
                  label="VCName *"
                  value={vcName}
                  onChange={this.vcNameChange}
                  margin="normal"
                  error={vcNameValidateObj.error}
                  fullWidth={true}
                  disabled={isEdit}
                  helperText={vcNameValidateObj.text}
                  inputProps={{ maxLength: 20 }}
                />
                <h3>DeviceNumber</h3>
                {Object.keys(allDevice).length > 0 && this.getSelectHtml(1)}
                <h3>MaxAvailable</h3>
                {Object.keys(allDevice).length > 0 && this.getSelectHtml(2)}
              </form>
            </DialogContent>
            <DialogActions>
              <Button onClick={this.onCloseDialog} color="primary" variant="outlined">Cancel</Button>
              <Button onClick={this.save} color="primary" variant="contained" disabled={btnLoading} style={{ marginLeft: 8 }}>
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
              <Button onClick={this.delete} color="secondary" variant="contained" disabled={btnLoading} style={{ marginLeft: 8 }}>
                {btnLoading && <CircularProgress size={20}/>}Delete
              </Button>
            </DialogActions>
          </Dialog>}
        </div>
      </Container>
    )
  }
}