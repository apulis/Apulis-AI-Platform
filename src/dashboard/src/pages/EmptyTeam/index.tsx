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
    console.log(teams)
    if(teams === undefined) {
      history.replace('/');
    } else if (teams.length > 0) {
      history.replace('/');
    }
  }, [teams, history])

  const onClick = () => {
    window.open("mailto://qianjiangyuan@aliyun.com", "_blank");
    return (
      <Redirect to="/" />
    )
  }
  return (
    <Box display="flex">
      <Dialog open>
        <DialogTitle>
          {"提示"}
        </DialogTitle>
        <DialogContent>
          <DialogContentText>
            {"您当前没有钱江源集群权限。请联系管理员或发邮件到qianjiangyuan@aliyun.com申请相关权限"}
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={onClick} color="primary">
            去发邮件
          </Button>
        </DialogActions>
      </Dialog>
    </Box>
  )
};

export default EmptyTeam;
