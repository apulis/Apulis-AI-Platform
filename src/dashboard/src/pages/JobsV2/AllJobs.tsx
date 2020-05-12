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
import Loading from '../../components/Loading';
import useActions from '../../hooks/useActions';
import ClusterContext from './ClusterContext';
import { renderId, renderGPU, sortGPU, renderDate, sortDate, renderStatus } from './tableUtils';
import PriorityField from './PriorityField';
import message from '../../utils/message';
import { pollInterval } from '../../const';
import useInterval from '../../hooks/useInterval';

const renderUser = (job: any) => job['userName'].split('@', 1)[0];
const getSubmittedDate = (job: any) => new Date(job['jobTime']);
const getStartedDate = (job: any) => new Date(job['jobStatusDetail'] && job['jobStatusDetail'][0]['startedAt']);
const getFinishedDate = (job: any) => new Date(job['jobStatusDetail'] && job['jobStatusDetail'][0]['finishedAt']);
const _renderId = (job: any) => renderId(job, 1);

interface JobsTableProps {
  title: string;
  jobs: any[];
}

const JobsTable: FunctionComponent<JobsTableProps> = ({ title, jobs }) => {
  const { cluster } = useContext(ClusterContext);
  const [pageSize, setPageSize] = useState(5);
  const onChangeRowsPerPage = useCallback((pageSize: number) => {
    setPageSize(pageSize);
  }, [setPageSize]);

  const renderPrioirty = useCallback((job: any) => (
    <PriorityField job={job}/>
  ), [])
  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'Id', type: 'string', field: 'jobId',
      render: _renderId, disableClick: true },
    { title: 'Name', type: 'string', field: 'jobName' },
    { title: 'Status', type: 'string', field: 'jobStatus', render: renderStatus },
    { title: 'Number of Device', type: 'numeric',
      render: renderGPU, customSort: sortGPU },
    { title: 'User', type: 'string', render: renderUser},
    { title: 'Preemptible', type: 'boolean', field: 'jobParams.preemptionAllowed'},
    { title: 'Priority', type: 'numeric',
      render: renderPrioirty, disableClick: true },
    { title: 'Submitted', type: 'datetime',
      render: renderDate(getSubmittedDate), customSort: sortDate(getSubmittedDate) },
    { title: 'Started', type: 'datetime',
      render: renderDate(getStartedDate), customSort: sortDate(getStartedDate) },
    { title: 'Finished', type: 'datetime',
      render: renderDate(getFinishedDate), customSort: sortDate(getFinishedDate) },
  ], [renderPrioirty]);
  const options = useMemo<Options>(() => ({
    actionsColumnIndex: -1,
    pageSize,
    padding: 'dense'
  }), [pageSize]);
  const { support, approve, kill, pause, resume } = useActions(cluster.id);
  const actions = [support, approve, pause, resume, kill];

  return (
    <MaterialTable
      title={title}
      columns={columns}
      data={jobs}
      options={options}
      actions={actions}
      onChangeRowsPerPage={onChangeRowsPerPage}
    />
  );
}

const AllJobs: FunctionComponent = () => {
  const { cluster } = useContext(ClusterContext);
  const { selectedTeam } = useContext(TeamsContext);
  const [limit, setLimit] = useState(100);
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
          if (!(temp1 === temp2)) setJobs(res.data);
        }, () => {
          message('error', `Failed to fetch jobs from cluster: ${cluster.id}`);
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
      {runningJobs && <JobsTable title="Running Jobs" jobs={runningJobs}/>}
      {queuingJobs && <JobsTable title="Queuing Jobs" jobs={queuingJobs}/>}
      {unapprovedJobs && <JobsTable title="Unapproved Jobs" jobs={unapprovedJobs}/>}
      {pausedJobs && <JobsTable title="Pauses Jobs" jobs={pausedJobs}/>}
      {jobs.length === 0 &&
        <h3 style={{marginLeft: '10px'}}>Only Running/Queuing/Unapproved/Pauses jobs will be shown and will not show Finished jobs</h3>
      }
    </>
  );

  return <Loading/>;
};

export default AllJobs;
