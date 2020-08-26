import React, { useEffect } from "react";
import * as H from 'history';
import { Box, Dialog, DialogTitle, DialogContent, DialogContentText, DialogActions, Button } from "@material-ui/core";
import { getPageQuery } from "../utils/getPageQuery";
import { withRouter, RouteComponentProps } from "react-router-dom";
import Loading from '../components/Loading';

interface IAuthContext {
  id?: string | number;
  openId?: string;
  userName?: string;
  currentRole?: string[];
  permissionList?: string[];
  userGroupPath?: string;
  team?: string[];
}

const AuthContext = React.createContext<IAuthContext>({});

export default AuthContext;

interface ProviderProps extends IAuthContext {
}

const Provider: React.FC<ProviderProps & RouteComponentProps> = ({ children, id, openId, userName, currentRole, permissionList, userGroupPath = '', history, team }) => {
  const isLogin = Boolean(id);
  const isRegister = Boolean(userName);

  const onClick = (path: string) => {
    const redirect = window.location.href.split('?')[0];
    window.location.href = userGroupPath + path + '?redirect=' + encodeURIComponent(redirect);
  }
  useEffect(() => {
    const { token } = getPageQuery();
    if (token) {
      localStorage.token = token;
      const { location } = history;
      history.push(location.pathname);
    }

  }, [])
  const href = window.location.href;
  if (!isLogin) {
    if (!/localhost/.test(href)) {
      onClick('/user/login');
      // return null;
    } else {
      alert('TO LOGIN！')
    }
    return (
      <AuthContext.Provider
        value={{ id, openId, userName, currentRole, permissionList }}
        children={<Loading />}
      />
    );
  }
  if (!isRegister) {
    if (!/localhost/.test(href)) {
      onClick('/user/register');
      // return null;
    } else {
      alert('TO LOGIN！')
    }
    return (
      <AuthContext.Provider
        value={{ id, openId, userName, currentRole, permissionList }}
        children={<Loading />}
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