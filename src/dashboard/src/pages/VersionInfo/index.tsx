import React, { useState, useEffect, useContext } from 'react';
import {
  Button, Divider
} from '@material-ui/core';
import Description from './components/Description'
import { useHistory } from 'react-router-dom';
import DrawerContext from '../../layout/Drawer/Context';
import ClusterContext from '../../contexts/Clusters';
import axios from 'axios';

interface VersionInfo {
  name: string,
  updateAt: string,
  description: string
}

const Version: React.FC = () => {
  const { selectedCluster } = useContext(ClusterContext);
  console.log('ssssssssssssss',selectedCluster)
  const history = useHistory();
  const [curVersion, setCurVersion] = useState<VersionInfo>({ name: '', updateAt: '', description: '' });
  const [historyVersionArr, setHistoryVersionArr] = useState([{ name: '', updateAt: '', description: '' }]);
  const { setOpen, open } = useContext(DrawerContext);

  const apiGetVersionInfo = async () => {
    const url = `/${selectedCluster}/VersionInfo`;
    await axios.get(url)
      .then((res: any) => {
        if (res) {
          const result = res.data;
          console.log(result);
          setCurVersion(result.version);
          setHistoryVersionArr(result.history);
        }
      })
  }

  useEffect(() => {
    apiGetVersionInfo();
  }, [])

  return (
    <>
      <div style={{ paddingLeft: '16px' }}>
        <Button variant="contained" color="primary" onClick={() => { history.push('/home'); setOpen(true) }}>返回首页</Button>
      </div>
      <div style={{ marginRight: '64px' }}>
        <h2 style={{ marginLeft: '100px', color: '#3f51b5' }}>当前版本：</h2>
        <Divider />
        <Description titleText='版本号：' timeText='更新时间:' descText='版本描述:' title={curVersion.name} time={curVersion.updateAt} desc={curVersion.description} />
      </div>
      { historyVersionArr.length && (<div style={{ marginRight: '64px' }}>
        <h2 style={{ marginLeft: '100px', color: '#3f51b5' }}>历史版本：</h2>
        <Divider />
        {
          historyVersionArr.map((version) => (
            <Description titleText='版本号：' timeText='更新时间:' descText='版本描述:' title={version.name} time={version.updateAt} desc={version.description} />
          ))
        }
      </div>)
      }
    </>
  )
};

export default Version;