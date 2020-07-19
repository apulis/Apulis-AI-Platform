import React, { useContext } from 'react';

import UserContext from '../contexts/User';


interface IAuthzHOC {
  needPermission?: string | string[];
}


const AuthzHOC: React.FC<IAuthzHOC> = ({children, needPermission}) => {
  if (!needPermission) {
    return (
      <>
        {children}
      </>
    )
  }
  const { permissionList = [] } = useContext(UserContext);
  let includes = false;
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
      <>
        {children}
      </>
    )
  } else {
    return null;
  }
  
}


export default AuthzHOC;