import React, { useEffect } from "react";
import * as H from 'history';
import { Box, Dialog, DialogTitle, DialogContent, DialogContentText, DialogActions, Button } from "@material-ui/core";
import { getPageQuery } from "../utils/getPageQuery";
import { withRouter, RouteComponentProps } from "react-router-dom";

interface IAuthContext {
  id?: string | number;
  openId?: string;
  userName?: string;
  currentRole?: string[];
  permissionList?: string[];
  userGroupPath?: string;
}

const AuthContext = React.createContext<IAuthContext>({});

export default AuthContext;

interface ProviderProps extends IAuthContext {
}

const Provider: React.FC<ProviderProps & RouteComponentProps> = ({ children, id, openId, userName, currentRole, permissionList, userGroupPath = '', history }) => {
  const isLogin = Boolean(id);
  const isRegister = Boolean(userName);

  let chidComponent: React.ReactNode;
  const onClick = (path: string) => {
    const redirect = window.location.href.split('?')[0];
    window.location.href = userGroupPath + path + '?redirect  =' + redirect;
  }
  useEffect(() => {
    const { token } = getPageQuery();
    if (token) {
      localStorage.token = token;
      const { location } = history;
      history.push(location.pathname);
    }

  }, [])
  if (!isLogin) {
    chidComponent = (<Box display="flex">
      <Dialog open>
        <DialogTitle style={{ color: 'red' }}>
          Notice
        </DialogTitle>
        <DialogContent style={{width: '350px'}}>
          <DialogContentText>
            Not login
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => onClick('/user/login')} color="primary">
            To Login
          </Button>
        </DialogActions>
      </Dialog>
    </Box>)
    return (
      <AuthContext.Provider
        value={{ id, openId, userName, currentRole, permissionList }}
        children={chidComponent}
      />
    );
  }
  if (!isRegister) {
    chidComponent = (<Box display="flex">
      <Dialog open>
        <DialogTitle style={{ color: 'red' }}>
          Notice
        </DialogTitle>
        <DialogContent style={{width: '350px'}}>
          <DialogContentText>
            Not Register
        </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => onClick('/user/register')} color="primary">
            To Register
        </Button>
        </DialogActions>
      </Dialog>
    </Box>)
    return (
      <AuthContext.Provider
        value={{ id, openId, userName, currentRole, permissionList }}
        children={chidComponent}
      />
    );
  }
  return (
    <AuthContext.Provider
      value={{ id, openId, userName, currentRole, permissionList }}
      children={children}
    />
  );

};

export const AuthProvider = withRouter(Provider);