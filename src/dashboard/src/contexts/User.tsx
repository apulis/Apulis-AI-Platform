import React from "react";

interface Context {
  email?: string;
  uid?: string;
  Alias?: string;
  Password?: string;
  token?: any;
}

const Context = React.createContext<Context>({});

export default Context;

interface ProviderProps {
  email?: string;
  uid?: string;
  Alias?: string;
  Password?: string;
  token?: any;
}

export const Provider: React.FC<ProviderProps> = ({ email, uid, Alias, Password, token, children }) => {
  if (token) {
    token = new Buffer(token.data).toString('hex');
  }
  return (
    <Context.Provider
      value={{ email,uid,Alias,Password, token }}
      children={children}
    />
  );
};
