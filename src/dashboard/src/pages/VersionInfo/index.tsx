import React, { useState, useEffect, useContext } from 'react';
import { Button, Divider } from '@material-ui/core';
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
  const history = useHistory();
  const [curVersion, setCurVersion] = useState<VersionInfo>({ name: '', updateAt: '', description: '' });
  const [historyVersionArr, setHistoryVersionArr] = useState([{ name: '', updateAt: '', description: '' }]);
  const { setOpen, open } = useContext(DrawerContext);

  const apiGetVersionInfo = async () => {
    const url = `/${selectedCluster}/VersionInfo`;
    debugger
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
    debugger
    apiGetVersionInfo();
  }, [])

  return (
    <>
      <div style={{ padding: '0 0 16px 16px' }}>
        <Button variant="contained" color="primary" onClick={() => { history.push('/home'); setOpen(true) }}>back home</Button>
      </div>
      <div style={{ marginRight: '64px', position: 'relative' }}>
        <Description titleText='version:' timeText='updateAt:' descText='description:' title={curVersion.name} time={curVersion.updateAt} desc={curVersion.description} icon={true} iconText={'current'}/>
        {
          historyVersionArr.map((version) => (
            <Description titleText='version:' timeText='updateAt:' descText='description:' title={version.name} time={version.updateAt} desc={version.description} />
          ))
        }
      </div>
    </>
  )
};

export default Version;