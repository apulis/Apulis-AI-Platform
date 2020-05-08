import React from "react";

interface Context {
  uid?: string;
  openId?: string;
  group?: string;
  userName?: string;
  nickName?: string;
  isAdmin?: boolean;
  isAuthorized?: boolean;
  email?: string;
  familyName?: string;
  givenName?: string;
  authEnabled?: {
    [propsName: string]: 1 | 0;
  };
  administrators?: Array<[]>
}

const Context = React.createContext<Context>({});

export default Context;

interface ProviderProps {
  uid?: string;
  openId?: string;
  group?: string;
  nickName?: string;
  userName?: string;
  isAdmin?: boolean;
  isAuthorized?: boolean;
  authEnabled?: {
    [propsName: string]: 0 | 1;
  };
  administrators?: Array<[]>
}

export const Provider: React.FC<ProviderProps> = ({ uid, openId, group, nickName, userName, isAdmin, isAuthorized, children, authEnabled, administrators }) => {
  return (
    <Context.Provider
      value={{ uid, openId, group, nickName, userName, isAdmin, isAuthorized, authEnabled, administrators }}
      children={children}
    />
  );
};
