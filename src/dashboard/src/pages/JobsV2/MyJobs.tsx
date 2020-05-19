import React, {
  FunctionComponent,
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useState
} from 'react';
import MaterialTable, { Column, Options } from 'material-table';
import { useSnackbar } from 'notistack';
import useFetch from 'use-http-2';
import _ from "lodash";
import TeamsContext from '../../contexts/Teams';
import useActions from '../../hooks/useActions';
import useInterval from '../../hooks/useInterval';
import Loading from '../../components/Loading';
import axios from 'axios';
import ClusterContext from './ClusterContext';
import { renderId, renderGPU, sortGPU, renderStatus, renderDate, sortDate } from './tableUtils';
import PriorityField from './PriorityField';
import { pollInterval } from '../../const';
import message from '../../utils/message';

const getSubmittedDate = (job: any) => new Date(job['jobTime']);
const getStartedDate = (job: any) => new Date(
  job['jobStatusDetail'] && job['jobStatusDetail'][0] ? job['jobStatusDetail'][0]['startedAt'] : undefined);
const getFinishedDate = (job: any) => new Date(
  job['jobStatusDetail'] && job['jobStatusDetail'][0] ? job['jobStatusDetail'][0]['finishedAt'] : undefined);
const _renderId = (job: any) => renderId(job, 0);

interface JobsTableProps {
  jobs: any[];
  onExpectMoreJobs?: (length: number) => void;
}

const JobsTable: FunctionComponent<JobsTableProps> = ({ jobs, onExpectMoreJobs }) => {
  const { cluster } = useContext(ClusterContext);
  const [pageSize, setPageSize] = useState(10);
  const onChangeRowsPerPage = useCallback((pageSize: number) => {
    setPageSize(pageSize);
  }, [setPageSize]);
  // const onChangePage = useCallback((page: number) => {
  //   const maxPage = Math.ceil(jobs.length / pageSize) - 1;
  //   if (page >= maxPage) {
  //     onExpectMoreJobs(pageSize);
  //   }
  // }, [jobs, pageSize, onExpectMoreJobs]);
  const renderPrioirty = useCallback((job: any) => (
    <PriorityField job={job}/>
  ), [])

  const columns = useMemo<Array<Column<any>>>(() => [
    { title: 'Id', type: 'string', field: 'jobId',
      render: _renderId, disableClick: true, sorting: false },
    { title: 'Name', type: 'string', field: 'jobName', sorting: false },
    { title: 'Status', type: 'string', field: 'jobStatus', sorting: false, render: renderStatus },
    { title: 'Number of Device', type: 'numeric',
      render: renderGPU, customSort: sortGPU },
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
    padding: 'dense',
    actionsColumnIndex: -1,
    pageSize
  }), [pageSize]);
  const { support, pause, resume, kill } = useActions(cluster.id);
  const actions = [support, pause, resume, kill];
  return (
    <MaterialTable
      title="My Jobs"
      columns={columns}
      data={jobs}
      options={options}
      actions={actions}
      onChangeRowsPerPage={onChangeRowsPerPage}
      // onChangePage={onChangePage}
    />
  );
};

const MyJobs: FunctionComponent = () => {
  const { enqueueSnackbar, closeSnackbar } = useSnackbar();
  const { cluster } = useContext(ClusterContext);
  const { selectedTeam } = useContext(TeamsContext);
  const [limit, setLimit] = useState(999);
  const [jobs, setJobs] = useState<any[]>();
  const onExpectMoreJobs = useCallback((count: number) => {
    setLimit((limit: number) => limit + count);
  }, []);

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
    axios.get(`/v2/clusters/${cluster.id}/teams/${selectedTeam}/jobs?limit=${limit}`)
      .then(res => {
        const { data } = res;
        const temp1 = JSON.stringify(jobs?.map(i => i.jobStatus));
        const temp2 = JSON.stringify(data?.map((i: { jobStatus: any; }) => i.jobStatus));
        if (!(temp1 === temp2)) setJobs(res.data);
      }, () => {
        message('error', `Failed to fetch jobs from cluster: ${cluster.id}`);
      })
  }

  if (jobs !== undefined) return (
    <JobsTable
      jobs={jobs}
      // onExpectMoreJobs={onExpectMoreJobs}
    />
  );

  return <Loading/>;
};

export default MyJobs;
