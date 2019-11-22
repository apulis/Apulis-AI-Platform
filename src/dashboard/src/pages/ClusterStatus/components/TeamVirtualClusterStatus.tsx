import React from "react";
import {
  Table,
  TableHead,
  TableRow, TableCell, TableBody, Radio, Button, Modal, makeStyles, TextField, Grid,
} from "@material-ui/core";
import { checkObjIsEmpty, sumValues } from "../../../utlities/ObjUtlities";
import { TeamVCTitles } from "../../../Constants/TabsContants";
import useFetch from "use-http/dist";

interface TeamVC {
  children?: React.ReactNode;
  vcStatus: any;
  selectedValue: string;
  handleChange: any;
}

function rand() {
  return Math.round(Math.random() * 20) - 10;
}

function getModalStyle() {
  const top = 50 + rand();
  const left = 50 + rand();

  return {
    top: `${top}%`,
    left: `${left}%`,
    transform: `translate(-${top}%, -${left}%)`,
  };
}

const useStyles = makeStyles(theme => ({
  paper: {
    position: 'absolute',
    width: 400,
    backgroundColor: theme.palette.background.paper,
    border: '2px solid #000',
    boxShadow: theme.shadows[5],
    padding: theme.spacing(2, 4, 3),
  },
  container: {
    display: 'flex',
    flexWrap: 'wrap',
  },
  textField: {
    marginLeft: theme.spacing(1),
    marginRight: theme.spacing(1),
    width: 200,
  },
}));

export const TeamVirtualClusterStatus = (props: TeamVC) => {
  const { vcStatus, selectedValue, handleChange } = props;
  const classes = useStyles();
  const [modalStyle] = React.useState(getModalStyle);
  const [open, setOpen] = React.useState(false);
  const [vcName, setVcName] = React.useState("");
  const [quota, setQuota] = React.useState("");
  const [metadata, setMetadata] = React.useState("");

  const onVcNameChange = React.useCallback(
    (event: React.ChangeEvent<HTMLInputElement>) => {
      setVcName(event.target.value);
    },
    [setVcName]
  );
  const onQuotaChange = React.useCallback(
    (event: React.ChangeEvent<HTMLInputElement>) => {
      setQuota(event.target.value);
    },
    [setQuota]
  );
  const onMetadataChange = React.useCallback(
    (event: React.ChangeEvent<HTMLInputElement>) => {
      setMetadata(event.target.value);
    },
    [setMetadata]
  );


  const handleOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  const options = {
    onMount: true
  }
  const fetchVcStatusUrl = `/api`;
  const request = useFetch(fetchVcStatusUrl, options);

  const addVc = async (event: React.FormEvent) => {
    event.preventDefault();
    const response = await request.get(`/addVc/qjydev/${vcName}/${quota}/${metadata}`);
  }; 
  
  return (
    <div>
      <Button variant="outlined" size="medium" color="primary" onClick={handleOpen}>新增VC</Button>
      <Table size={"small"}>
        <TableHead>
          <TableRow>
            {
              TeamVCTitles.map((teamVCTitle, index) => (
                <TableCell key={index}>{teamVCTitle}</TableCell>
              ))
            }
          </TableRow>
        </TableHead>
        <TableBody>
          {
            vcStatus ? vcStatus.map((vcs: any, index: number) => {
              const gpuCapacity = checkObjIsEmpty(Object.values(vcs['gpu_capacity'])) ? 0 : (Number)(sumValues(vcs['gpu_capacity']));
              const gpuAvailable = checkObjIsEmpty(Object.values(vcs['gpu_avaliable'])) ? 0 : (Number)(sumValues(vcs['gpu_avaliable']));
              const gpuUnschedulable = checkObjIsEmpty(Object.values(vcs['gpu_unschedulable'])) ? 0 : (Number)(sumValues(vcs['gpu_unschedulable']));
              const gpuUsed = checkObjIsEmpty(Object.values(vcs['gpu_used'])) ? 0 : (Number)(sumValues(vcs['gpu_used']));
              return (
                <TableRow key={index}>
                  <TableCell>
                    <Radio
                      checked={selectedValue === vcs['ClusterName']}
                      onChange={handleChange}
                      value={vcs['ClusterName']}
                      name={vcs['ClusterName']} />
                    {vcs['ClusterName']}
                  </TableCell>
                  <TableCell>
                    {gpuCapacity}
                  </TableCell>
                  <TableCell>
                    {gpuUnschedulable}
                  </TableCell>
                  <TableCell>
                    {gpuUsed}
                  </TableCell>
                  <TableCell>
                    {gpuAvailable}
                  </TableCell>
                  <TableCell>
                    {vcs['AvaliableJobNum']}
                  </TableCell>
                </TableRow>
              )
            }) : null
          }
        </TableBody>
      </Table>
      <Modal
        aria-labelledby="simple-modal-title"
        aria-describedby="simple-modal-description"
        open={open}
        onClose={handleClose}
      >
        <div style={modalStyle} className={classes.paper}>
          <h2 id="simple-modal-title">add a new vc</h2>
          <form onSubmit={addVc}>
            <Grid item xs={12} sm={6}>
              <TextField
                required
                label="vcName"
                value={vcName}
                onChange={onVcNameChange}
                className={classes.textField}
                margin="normal"
              />
            </Grid>
            <Grid item xs={12} sm={6}>
              <TextField
                required
                label="quota"
                value={quota}
                onChange={onQuotaChange}
                className={classes.textField}
                margin="normal"
              />
            </Grid>
            <Grid item xs={12} sm={6}>
              <TextField
                required
                label="metadata"
                value={metadata}
                onChange={onMetadataChange}
                className={classes.textField}
                margin="normal"
              />
            </Grid>
            <Grid item xs={12} sm={6}>
              <Button variant="outlined" size="medium" color="primary" type="submit">Save</Button>
            </Grid>
          </form>
        </div>
      </Modal>
    </div>
  )
}
