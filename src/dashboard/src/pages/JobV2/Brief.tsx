import React, {
  FunctionComponent,
  useContext
} from 'react';
import {
  List,
  ListItem,
  ListItemText
} from '@material-ui/core';
import {
  Check,
  Close
} from '@material-ui/icons';

import CopyableTextListItem from '../../components/CopyableTextListItem';
import JobStatus from '../../components/JobStatus';

import Context from './Context';
import { useTranslation } from "react-i18next";

const Brief: FunctionComponent = () => {
  const {t} = useTranslation();
  const { cluster, job } = useContext(Context);
  return (
    <List dense>
      <ListItem>
        <ListItemText primary={t('jobV2.jobId')} secondary={job['jobId']}/>
      </ListItem>
      <ListItem>
        <ListItemText primary={t('jobV2.jobName')} secondary={job['jobName']}/>
      </ListItem>
      <ListItem>
        <ListItemText primary={t('jobV2.vcName')} secondary={job['vcName']}/>
      </ListItem>
      <ListItem>
        <ListItemText primary={t('jobV2.dockerImage')} secondary={job['jobParams']['image']}/>
      </ListItem>
      <ListItem>
        <ListItemText
          primary={t('jobV2.command')}
          secondary={job['jobParams']['cmd']}
          secondaryTypographyProps={{ component: 'div' }}
          style={{
            wordWrap:'break-word',
            wordBreak:'break-all',
            overflow: 'hidden'
          }}
        />
      </ListItem>
      <CopyableTextListItem
        primary={t('jobV2.dataPath')}
        secondary={`${cluster['dataStorage'] || ''}/${job['jobParams']['dataPath']}`}
      />
      <CopyableTextListItem
        primary={t('jobV2.workPath')}
        secondary={`${cluster['workStorage'] || ''}/${job['jobParams']['workPath']}`}
      />
      <CopyableTextListItem
        primary={t('jobV2.jobPath')}
        secondary={`${cluster['workStorage'] || ''}/${job['jobParams']['jobPath']}`}
      />
      <ListItem>
        <ListItemText
          primary={t('jobV2.preemptible')}
          secondary={job['jobParams']['preemptionAllowed'] ? <Check/> : <Close/>}
        />
      </ListItem>
      {
        job['jobParams']['jobtrainingtype'] === 'PSDistJob' && (
          <ListItem>
            <ListItemText
              primary={t('jobV2.numberOfNodes')}
              secondary={job['jobParams']['numpsworker']}
            />
          </ListItem>
        )
      }
      {
        job['jobParams']['jobtrainingtype'] === 'PSDistJob' && (
          <ListItem>
            <ListItemText
              primary={t('jobV2.totalOfDevice')}
              secondary={job['jobParams']['numpsworker'] * job['jobParams']['resourcegpu']}
            />
          </ListItem>
        )
      }
      {
        job['jobParams']['jobtrainingtype'] === 'RegularJob' && (
          <ListItem>
            <ListItemText
              primary={t('jobV2.deviceType')}
              secondary={job['jobParams']['gpuType']}
            />
          </ListItem>
        )
      }
      {
        job['jobParams']['jobtrainingtype'] === 'RegularJob' && (
          <ListItem>
            <ListItemText
              primary={t('jobV2.numberOfDevice')}
              secondary={job['jobParams']['resourcegpu']}
            />
          </ListItem>
        )
      }
      <ListItem>
        <ListItemText
          primary={t('jobV2.jobStatus')}
          secondary={<JobStatus job={job}/>}
          secondaryTypographyProps={{ component: 'div' }}
        />
      </ListItem>
      <ListItem>
        <ListItemText
          primary={t('jobV2.jobSubmissionTime')}
          secondary={new Date(job['jobTime']).toLocaleString('en')}
        />
      </ListItem>
    </List>
  );
};

export default Brief;
