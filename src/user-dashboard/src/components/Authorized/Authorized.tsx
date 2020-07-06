import React from 'react';
import { Result } from 'antd';
import check, { IAuthorityType } from './CheckPermissions';

import AuthorizedRoute from './AuthorizedRoute';
import Secured from './Secured';

interface AuthorizedProps {
  authority: IAuthorityType;
  noMatch?: React.ReactNode;
}

type IAuthorizedType = React.FunctionComponent<AuthorizedProps> & {
  Secured: typeof Secured;
  check: typeof check;
  AuthorizedRoute: typeof AuthorizedRoute;
};

const Authorized: React.FunctionComponent<AuthorizedProps> = ({
  children,
  authority,//哪个角色能渲染我这个组件
  noMatch = (//如果说权限没有匹配上怎么办，渲染谁?
    <Result
      status="403"
      title="403"
      subTitle="Sorry, you are not authorized to access this page."
    />
  ),
}) => {
  const childrenRender: React.ReactNode = typeof children === 'undefined' ? null : children;
  const dom = check(authority, childrenRender, noMatch);
  return <>{dom}</>;
};
/**
 * check 是一个函数，用来决定是否会渲染组件
 * authority 是组件属性，表示要渲染我需要什么角色 字符串 也可能是一个数组 ['admin','user']
 * return checkPermissions<T, K>(authority, CURRENT, target, Exception)
 * authority=['admin','user']
 * CURRENT=当前用户的角色数组 ["admin"]
 * target 要渲染的目标组件 权限判断成功了，渲染哪个组件
 * Exception 鉴权 失败了，没 有权限访问，需要渲染哪个组件
 * 
 * 
 * 从头理
 * 1.用户登录  服务器返回currentAuthority ,pro会转成数组存放到  local里 antd-pro-authority=['user']
 * 2.刷新权限  ['user'] 赋给CURRENT
 * 3.鉴权组件(Authorized)会重新刷新,如果角色匹配就会渲染，如果不匹配，则渲染noMatch组件 
 */

export default Authorized as IAuthorizedType;
