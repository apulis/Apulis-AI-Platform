import React from "react";

interface Context {
  email?: string;
  uid?: string;
  Alias?: string;
  Password?: string;
  token?: any;
  isAdmin?: boolean;
}

const Context = React.createContext<Context>({});

export default Context;

interface ProviderProps {
  email?: string;
  uid?: string;
  Alias?: string;
  Password?: string;
  token?: any;
  isAdmin?: boolean;
}

export const Provider: React.FC<ProviderProps> = ({ email, uid, Alias, Password, token, isAdmin, children }) => {
  if (token) {
    token = new Buffer(token.data).toString('hex');
  }
  return (
    <Context.Provider
      value={{ email, uid, Alias, Password, token, isAdmin }}
      children={children}
    />
  );
};
