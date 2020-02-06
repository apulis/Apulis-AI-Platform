import React from "react";

interface Context {
  uid?: string;
  openId?: string;
  group?: string;
  userName?: string;
  nickName?: string;
  password?: string;
  isAdmin?: boolean;
  isAuthorized?: boolean;
  token?: any;
  email?: string;
  familyName?: string;
  givenName?: string;
}

const Context = React.createContext<Context>({});

export default Context;

interface ProviderProps {
  uid?: string;
  openId?: string;
  group?: string;
  nickName?: string;
  userName?: string;
  password?: string;
  isAdmin?: boolean;
  isAuthorized?: boolean;
  token?: any;
}

export const Provider: React.FC<ProviderProps> = ({ uid, openId, group, nickName, userName, password, isAdmin, isAuthorized, children, token }) => {
  if (token) {
    token = new Buffer(token.data).toString('hex');
  }
  return (
    <Context.Provider
      value={{ uid, openId, group, nickName, userName, password, isAdmin, isAuthorized, token }}
      children={children}
    />
  );
};
