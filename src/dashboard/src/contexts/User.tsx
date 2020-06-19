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
  administrators?: Array<[]>;
  permissionList?: string[];
  currentRole?: string[];
  userGroupPath?: string;
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
  administrators?: Array<[]>;
  permissionList?: string[];
  currentRole?: string[];
  userGroupPath?: string;
}

export const Provider: React.FC<ProviderProps> = ({ uid, openId, group, nickName, userName, isAdmin, isAuthorized, children, administrators, currentRole, permissionList, userGroupPath }) => {
  return (
    <Context.Provider
      value={{ uid, openId, group, nickName, userName, isAdmin, isAuthorized, administrators, currentRole, permissionList, userGroupPath }}
      children={children}
    />
  );
};
