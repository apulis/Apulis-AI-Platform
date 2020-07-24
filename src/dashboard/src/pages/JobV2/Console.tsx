import React, {
  FunctionComponent,
  useEffect,
  useMemo,
  useState
} from 'react';
import {
  Box,
} from '@material-ui/core';
import { useParams } from 'react-router-dom';
import useFetch from 'use-http-2';
import { useSnackbar } from 'notistack';
import { Pagination } from '@material-ui/lab';
import { pollInterval } from '../../const';
import useInterval from '../../hooks/useInterval';
import message from '../../utils/message';
import axios from 'axios';

interface RouteParams {
  clusterId: string;
  jobId: string;
}

const Console: FunctionComponent = () => {
  const { clusterId, jobId } = useParams<RouteParams>();
  const [data, setData] = useState({});
  const [count, setCount] = useState(0);
  const [page, setPage] = useState(1);

  useEffect(() => {
    getData();
  }, [clusterId, jobId, page]);

  useInterval(() => {
    getData();
  }, pollInterval);

  const getData = () => {
    axios.get(`/clusters/${clusterId}/jobs/${jobId}/log?page=${page}`)
      .then((res: any) => {
        const { log, max_page } = res.data;
        max_page !== count && setCount(max_page);
        log !== data && setData(log);
      }, () => {
        message('error', `Failed to fetch job log: ${clusterId}/${jobId}`);
      })
  }

  if (Object.keys(data).length === 0) {
    return (
      <Box p={1} style={{ overflow: 'auto' }}>
        <Box m={0} component="pre">
          Because this job has not started, there is no logs
        </Box>
      </Box>
    )
  }

  return (
    <Box p={1} style={{ overflow: 'auto', padding: 20 }}>
      <Box m={0} component="pre">
        {data}
        {count && <Pagination
          color="primary"
          count={count}
          page={page}
          style={{ float: 'right' }}
          onChange={(e: any, page: number) => setPage(page)}
        />}
      </Box>
    </Box>
  );
}

export default Console;
