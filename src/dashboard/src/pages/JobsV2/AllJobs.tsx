import React, {
  FunctionComponent,
  useCallback,
  useContext,
  useState,
  useEffect,
  useMemo
} from 'react';
import MaterialTable, { Column, Options } from 'material-table';
import axios from 'axios';
import TeamsContext from '../../contexts/Teams';
import AuthContext from '../../contexts/Auth';
import Loading from '../../components/Loading';
import useActions from '../../hooks/useActions';
import ClusterContext from './ClusterContext';
import { renderId, renderGPU, sortGPU, renderDate, sortDate, renderStatus } from './tableUtils';
import PriorityField from './PriorityField';
import message from '../../utils/message';
import { pollInterval } from '../../const';
import useInterval from '../../hooks/useInterval';
import { useTranslation } from "react-i18next";

const renderUser = (job: any) => job['userName'].split('@', 1)[0];
const getSubmittedDate = (job: any) => new Date(job['jobTime']);
const getStartedDate = (job: any) => new Date(job['jobStatusDetail'] ? job['jobStatusDetail'][0]['startedAt'] : undefined);
const getFinishedDate = (job: any) => new Date(job['jobStatusDetail'] ? job['jobStatusDetail'][0]['finishedAt'] : undefined);
const _renderId = (job: any) => renderId(job, 1);

interface JobsTableProps {
  title: string;
  jobs: any[];
}

const JobsTable: FunctionComponent<JobsTableProps> = ({ title, jobs }) => {
  const {t} = useTranslation();
  const { cluster } = useContext(ClusterContext);
  const { permissionList = [] } = useContext(AuthContext);
  const [pageSize, setPageSize] = useState(5);
  const onChangeRowsPerPage = useCallback((pageSize: number) => {
    setPageSize(pageSize);
  }, [setPageSize]);
  const columns: Column<any>[]  = [
    { title: t('jobsV2.id'), type: 'string', field: 'jobId',
      render: _renderId, disableClick: true, sorting: false, cellStyle: {fontFamily: 'Lucida Console'}},
    { title: t('jobsV2.name'), type: 'string', field: 'jobName', sorting: false },
    { title: t('jobsV2.status'), type: 'string', field: 'jobStatus', render: renderStatus, sorting: false },
    { title: t('jobsV2.deviceNumber'), type: 'numeric',
      render: renderGPU, customSort: sortGPU },
    { title: t('jobsV2.user'), type: 'string', render: renderUser},
    { title: t('jobsV2.preemptible'), type: 'boolean', field: 'jobParams.preemptionAllowed'},
    { title: t('jobsV2.priority'), type: 'numeric',
      render: (job: any) => (<PriorityField job={job} key={job.jobId} />), disableClick: true },
    { title: t('jobsV2.submitted'), type: 'datetime',
      render: renderDate(getSubmittedDate), customSort: sortDate(getSubmittedDate) },
    { title: t('jobsV2.started'), type: 'datetime',
      render: renderDate(getStartedDate), customSort: sortDate(getStartedDate) },
    { title: t('jobsV2.finished'), type: 'datetime',
      render: renderDate(getFinishedDate), customSort: sortDate(getFinishedDate) },
  ];
  const options = useMemo<Options>(() => ({
    actionsColumnIndex: -1,
    pageSize,
    padding: 'dense'
  }), [pageSize]);
  const { supportEmail, approve, kill, pause, resume } = useActions(cluster.id);
  const actions = [supportEmail, approve, pause, resume, kill];

  return (
    <MaterialTable
      title={title}
      columns={columns}
      data={jobs}
      options={options}
      actions={permissionList.includes('VIEW_AND_MANAGE_ALL_USERS_JOB') ? actions : undefined}
      onChangeRowsPerPage={onChangeRowsPerPage}
      localization={{
        pagination: {
            labelDisplayedRows: '{from}-{to} of {count}',
            firstTooltip: t('jobsV2.firstPage'),
            previousTooltip: t('jobsV2.previousPage'),
            nextTooltip: t('jobsV2.nextPage'),
            lastTooltip: t('jobsV2.lastPage'),
            labelRowsPerPage: t('jobsV2.labelRowsPerPage'),
            labelRowsSelect: t('jobsV2.labelRowsSelect')
        },
        toolbar: {
            nRowsSelected: '{0} row(s) selected',
            searchTooltip: t('jobsV2.search'),
            searchPlaceholder: t('jobsV2.search')
        },
        header: {
            actions: t('jobsV2.actions')
        },
        body: {
            emptyDataSourceMessage: t('jobsV2.noRecordsToDisplay'),
            filterRow: {
                filterTooltip: 'Filter'
            }
        }
      }}  
    />
  );
}

const AllJobs: FunctionComponent = () => {
  const {t} = useTranslation();
  const { cluster } = useContext(ClusterContext);
  const { selectedTeam } = useContext(TeamsContext);
  const [limit, setLimit] = useState(9999);
  const [jobs, setJobs] = useState<any[]>();
  useEffect(() => {
    setJobs(undefined);
  }, [cluster.id]);

  useEffect(() => {
    getData();
  }, [cluster.id, selectedTeam, limit]);

  useInterval(() => {
    getData();
  }, pollInterval);

  const getData = () => {
    axios.get(`/v2/clusters/${cluster.id}/teams/${selectedTeam}/jobs?user=all&limit=${limit}`)
      .then(res => {
        const { data } = res;
        const temp1 = JSON.stringify(jobs?.map(i => i.jobStatus));
        const temp2 = JSON.stringify(data?.map((i: { jobStatus: any; }) => i.jobStatus));
        if (!(temp1 === temp2))  setJobs(res.data);
      }, () => {
        message('error', `${t('tips.Failedtofetchjobsfromcluster')}: ${cluster.id}`);
      })
  }

  const runningJobs = useMemo(() => {
    if (jobs === undefined) return undefined;
    const runningJobs = jobs.filter((job: any) => job['jobStatus'] === 'running');
    if (runningJobs.length === 0) return undefined;
    return runningJobs;
  }, [jobs]);
  const queuingJobs = useMemo(() => {
    if (jobs === undefined) return undefined;
    const queuingJobs = jobs.filter((job: any) => job['jobStatus'] === 'queued' || job['jobStatus'] === 'scheduling' );
    if (queuingJobs.length === 0) return undefined;
    return queuingJobs
  }, [jobs]);
  const unapprovedJobs = useMemo(() => {
    if (jobs === undefined) return undefined;
    const unapprovedJobs = jobs.filter((job: any)=>job['jobStatus'] === 'unapproved');
    if (unapprovedJobs.length === 0) return undefined;
    return unapprovedJobs
  }, [jobs]);
  const pausedJobs = useMemo(() => {
    if (jobs === undefined) return undefined;
    const pausedJobs = jobs.filter((job: any) => job['jobStatus'] === 'paused' || job['jobStatus'] === 'pausing' );
    if (pausedJobs.length === 0) return undefined;
    return pausedJobs
  }, [jobs]);
  if (jobs !== undefined) return (
    <>
      {runningJobs && <JobsTable title={t('jobsV2.runningJobs')} jobs={runningJobs}/>}
      {queuingJobs && <JobsTable title={t('jobsV2.queueingJobs')} jobs={queuingJobs}/>}
      {unapprovedJobs && <JobsTable title={t('jobsV2.unapprovedJobs')} jobs={unapprovedJobs}/>}
      {pausedJobs && <JobsTable title={t('jobsV2.pauseJobs')} jobs={pausedJobs}/>}
      {jobs.length === 0 &&
        <h3 style={{marginLeft: '10px'}}>{t('tips.OnlyRunningOrQueuingOrUnapprovedOrPausesjobswillbeshownandwillnotshowFinishedjobs')}</h3>
      }
    </>
  );

  return <Loading/>;
};

export default AllJobs;
