import React, {
  FunctionComponent,
  KeyboardEvent,
  useCallback,
  useContext,
  useMemo,
  useState,
  useRef
} from 'react';
import { Button, TextField } from '@material-ui/core';
import { useSnackbar } from 'notistack';
import ClusterContext from './ClusterContext';
import AuthContext from '../../contexts/Auth';


interface Props {
  job: any;
}

const PriorityField: FunctionComponent<Props> = ({ job }) => {
  const { enqueueSnackbar } = useSnackbar();
  const { cluster } = useContext(ClusterContext);
  const { permissionList = [] } = useContext(AuthContext);
  const [editing, setEditing] = useState(false);
  const [textFieldDisabled, setTextFieldDisabled] = useState(false);
  // const input = useRef<HTMLInputElement>();
  const buttonEnabled = useMemo(() => {
    return (
      job['jobStatus'] === 'running' ||
      job['jobStatus'] === 'queued' ||
      job['jobStatus'] === 'scheduling' ||
      job['jobStatus'] === 'unapproved' ||
      job['jobStatus'] === 'paused' ||
      job['jobStatus'] === 'pausing'
    )
  }, [job])
  const [priority, setPriority] = useState(Number(job['priority']) || 100);
  const onBlur = (event: KeyboardEvent<HTMLInputElement>) => {
    setEditing(false);
    const val = priority < 1 ? 1 : priority > 1000 ? 1000 : priority;
    setPriority(val);
    if (val === job['priority']) return;
    enqueueSnackbar('Priority is being set...');
    setTextFieldDisabled(true);

    fetch(`/api/clusters/${cluster.id}/jobs/${job['jobId']}/priority`, {
      method: 'PUT',
      body: JSON.stringify({ priority: val }),
      headers: { 'Content-Type': 'application/json' }
    }).then((response) => {
      if (response.ok) {
        enqueueSnackbar('Priority is set successfully', { variant: 'success' });
      } else {
        throw Error();
      }
      setEditing(false);
    }).catch(() => {
      enqueueSnackbar('Failed to set priority', { variant: 'error' });
    }).then(() => {
      setTextFieldDisabled(false);
    });
  }

  if (editing) {
    return (
      <TextField
        // inputRef={input}
        type="number"
        value={priority}
        disabled={textFieldDisabled || !permissionList.includes('MANAGE_ALL_USERS_JOB')}
        fullWidth
        onBlur={onBlur}
        onChange={e => setPriority(Number(e.target.value))}
      />
    );
  } else {
    return (
      <Button
        fullWidth
        variant={buttonEnabled ? 'outlined' : 'text'}
        onClick={buttonEnabled ? () => setEditing(true) : undefined}
        disabled={!permissionList.includes('MANAGE_ALL_USERS_JOB')}
      >
        {priority}
      </Button>
    );
  }
};

export default PriorityField;
