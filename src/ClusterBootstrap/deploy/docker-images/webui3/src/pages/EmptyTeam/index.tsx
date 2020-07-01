import React from 'react';
import { RouteComponentProps } from "react-router-dom"
import {
  Box, Button,
  Dialog, DialogActions,
  DialogContent,
  DialogContentText,
  DialogTitle
} from "@material-ui/core";
import { Redirect } from "react-router";

import TeamsContext from "../../contexts/Teams";

const EmptyTeam: React.FC<RouteComponentProps> = ({ history }) => {
  const { teams } = React.useContext(TeamsContext);

  React.useEffect(() => {
    if(teams === undefined) {
      history.replace('/');
    } else if (teams.length > 0) {
      const redict = window.location.search.split('redict=')[1] || '/';
      history.replace(redict);
    }
  }, [teams, history])

  const onClick = () => {
    window.open("mailto://bifeng.peng@apulis.com", "_blank");
    return (
      <Redirect to="/" />
    )
  }
  return (
    <Box display="flex">
      <Dialog open>
        <DialogTitle>
          {"Notice"}
        </DialogTitle>
        <DialogContent>
          <DialogContentText>
            {"You currently do not have permission to use it, please contact your system administrator to add you to the group"}
          </DialogContentText>
        </DialogContent>
        {/* <DialogActions>
          <Button onClick={onClick} color="primary">
            去发邮件
          </Button>
        </DialogActions> */}
      </Dialog>
    </Box>
  )
};

export default EmptyTeam;
