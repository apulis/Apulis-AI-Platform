import React from "react";
import { Box, Dialog, DialogTitle, DialogContent, DialogContentText, DialogActions, Button } from "@material-ui/core";

interface IAuthContext {
  id?: string | number;
  openId?: string;
  userName?: string;
  currentRole?: string[];
  permissionList?: string[];
}

const AuthContext = React.createContext<IAuthContext>({});

export default AuthContext;

interface ProviderProps extends IAuthContext {
}

export const AuthProvider: React.FC<ProviderProps> = ({ children, id, openId, userName, currentRole, permissionList }) => {
  const isLogin = Boolean(id);
  const isRegister = Boolean(userName);

  let chidComponent: React.ReactNode;
  const onClick = (path: string) => {
    console.log(path)
  }
  if (!isLogin) {
    chidComponent = (<Box display="flex">
      <Dialog open>
        <DialogTitle style={{ color: 'red' }}>
          {"warning"}
        </DialogTitle>
        <DialogContent>
          <DialogContentText>
            Not login
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => onClick('login')} color="primary">
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
          {"warning"}
        </DialogTitle>
        <DialogContent>
          <DialogContentText>
            Not Register
        </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => onClick('login')} color="primary">
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
