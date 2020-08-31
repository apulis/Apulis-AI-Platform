import React, { useState, useEffect,useContext } from 'react';
import {
  Button,
} from '@material-ui/core';
import Description from './components/Description'
import { useHistory } from 'react-router-dom';
import DrawerContext from '../../layout/Drawer/Context';

interface VersionInfo {
  version: string,
  updateAt: string,
  desc: string
}

const Version:React.FC = () => {
  const history = useHistory();
  const [versionInfo, setVersionInfo] = useState<VersionInfo>({version: '',
    updateAt: '',
    desc: ''})
  const { setOpen, open } = useContext(DrawerContext);
  useEffect(() => {
    renderInit()
  }, [])
  const renderInit = async () => {
    const result = await apiGetVersionInfo()
    if (result) {
      setVersionInfo(result)
    }
  }
  const apiGetVersionInfo = async () => {
    return {
      version: '11.0.1',
      updateAt: '2020/08/20 12:32',
      desc: '当前版本号：1.0.1当前版本号：'
    }
  }
  return (
    <>
    <div style={{paddingLeft:'16px'}}>
    <Button variant="contained" color="primary" onClick={()=>{history.push('/home');setOpen(true)}}>返回首页</Button>
    </div>
    <div style={{marginRight: '64px'}}>
    <Description titleText='版本号：' timeText='更新时间:' descText='版本描述:' title={versionInfo.version} time={versionInfo.updateAt} desc={versionInfo.desc} />
    </div>
    </>
  )
};

export default Version;