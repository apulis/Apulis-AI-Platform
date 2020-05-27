import React, { ComponentType, useContext, FunctionComponent  } from 'react';
import { Route, RouteComponentProps } from 'react-router-dom';
import AuthContext from '../contexts/Auth';

interface IAuthzRouteProps {
  path: string;
  exact?: boolean;
  component:  ComponentType<RouteComponentProps<any>> | ComponentType<any>;
  needPermission?: string | string[];
  strict?: boolean
}

const AuthzRoute: FunctionComponent<IAuthzRouteProps> = ({ needPermission, path, exact, component, strict }) => {
  const { permissionList = [] } = useContext(AuthContext);
  let includes = false;
  if (!needPermission) {
    return <Route
      path={path}
      exact={exact}
      component={component}
      strict={strict}
    />
  }
  if (Array.isArray(needPermission)) {
    needPermission.forEach(p => {
      if (permissionList?.includes(p)) {
        includes = true;
      }
    })
  } else if (permissionList.includes(needPermission)) {
    includes = true;
  }
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