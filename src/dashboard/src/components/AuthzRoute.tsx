import React from 'react';
import { Route, RouteComponentProps } from 'react-router-dom';
import AuthContext from '../contexts/Auth';

interface IAuthzRouteProps {
  path: string;
  exact?: boolean;
  component:  React.ComponentType<RouteComponentProps<any>> | React.ComponentType<any>;
  needRole?: string | string[];
  strict?: boolean
}


const AuthzRoute: React.FC<IAuthzRouteProps> = ({ needRole, path, exact, component, strict }) => {
  const { currentRole = [] } = React.useContext(AuthContext);
  console.log('current', currentRole)
  let includes = false;
  if (!needRole) {
    return <Route
      path={path}
      exact={exact}
      component={component}
      strict={strict}
    />
  }
  if (Array.isArray(needRole)) {
    needRole.forEach(p => {
      if (currentRole?.includes(p)) {
        includes = true;
      }
    })
  } else if (currentRole.includes(needRole)) {
    includes = true;
  }
  console.log('includes', includes)
  if (includes) {
    return (
      <Route
        path={path}
        exact={exact}
        component={component}
        strict={strict}
      />
    )
  } else {
    return null;
  }
}

export default AuthzRoute;