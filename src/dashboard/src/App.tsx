import React from 'react';
import { BrowserRouter, Redirect, Route, RouteComponentProps, Switch } from "react-router-dom"
import 'typeface-roboto';
import 'typeface-roboto-mono';
import Helmet from 'react-helmet';
import { Box, CssBaseline, createMuiTheme, CircularProgress } from '@material-ui/core';
import * as H from 'history';
import { ThemeProvider } from "@material-ui/styles";

import i18n from "i18next";
import { useTranslation, initReactI18next } from "react-i18next";
import initAxios from './utils/init-axios'
import ConfigContext, { Provider as ConfigProvider } from "./contexts/Config";
import UserContext, { Provider as UserProvider } from "./contexts/User";
import { Provider as ClustersProvider } from "./contexts/Clusters";
import TeamsContext, { Provider as TeamProvider } from './contexts/Teams';
import AuthContext, { AuthProvider } from './contexts/Auth';
import { ConfirmProvider } from './hooks/useConfirm';
import AppBar from "./layout/AppBar";
import Content from "./layout/Content";
import Drawer from "./layout/Drawer";
import { Provider as DrawerProvider } from "./layout/Drawer/Context";
import { SnackbarProvider, useSnackbar, VariantType } from 'notistack';
import './App.less';
import AuthzRoute from './components/AuthzRoute';
import ROUTER from './router.config';

import en from './locale/en-US';
import cn from './locale/zh-CN';

const theme = createMuiTheme();

interface BootstrapProps {
  uid?: string;
  openId?: string;
  group?: string;
  nickName?: string;
  userName?: string;
  isAdmin?: boolean;
  isAuthorized?: boolean;
  config: ConfigContext;
  user: UserContext;
  administrators?: Array<[]>;
  permissionList?: string[];
  currentRole?: string[];
  userGroupPath?: string;
  id?: number;
}

const lng = localStorage.language || navigator.language;

i18n
  .use(initReactI18next) // passes i18n down to react-i18next
  .init({
    resources: {
      "en-US": {
        translation: {
          en
        }
      },
      "zh-CN": {
        translation: {
          cn
        }
      }
    },
    lng: lng,
    fallbackLng: lng,

    interpolation: {
      escapeValue: false
    }
  });

const Loading = (
  <Box flex={1} display="flex" alignItems="center" justifyContent="center">
    <CircularProgress />
  </Box>
);

const Contexts: React.FC<BootstrapProps> = ({ uid, id, openId, group, nickName, userName, isAdmin, isAuthorized, children, administrators, permissionList, currentRole, userGroupPath }) => {
  const { enqueueSnackbar } = useSnackbar();
  initAxios((type: VariantType, msg: string) => {
    enqueueSnackbar(msg, {
      autoHideDuration: 3000,
      variant: type,
    });
  }, userGroupPath || '');
  console.log('permissionList',permissionList)
  return(
    <BrowserRouter>
      <ConfigProvider>
        <UserProvider uid={uid} openId={openId} group={group} nickName={nickName} userName={userName} isAdmin={isAdmin} isAuthorized={isAuthorized} administrators={administrators} permissionList={permissionList} currentRole={currentRole} userGroupPath={userGroupPath} >
          <ConfirmProvider>
            <AuthProvider userName={userName} id={id} userGroupPath={userGroupPath} permissionList={permissionList} currentRole={currentRole}>
              <TeamProvider permissionList={permissionList}>
                <ClustersProvider>
                  <ThemeProvider theme={theme}>
                    {children}
                  </ThemeProvider>
                </ClustersProvider>
              </TeamProvider>
            </AuthProvider>
          </ConfirmProvider>
        </UserProvider>
      </ConfigProvider>
    </BrowserRouter>
  )
};
const Layout: React.FC<RouteComponentProps> = ({ location, history }) => {
  
  return (
    <DrawerProvider>
      <Content>
        <AppBar />
        <Drawer />
        <React.Suspense fallback={Loading}>
          <Switch location={location}>
            {ROUTER.map(r => {
              return (
                <AuthzRoute
                  exact={r.exact}
                  key={r.path}
                  component={r.component}
                  strict={r.strict}
                  path={r.path}
                  needPermission={r.needPermission}
                />
              )
            })}
            <Redirect strict exact from="/jobs-v2/:clusterId" to="/jobs-v2/:clusterId/"/>
            <Redirect strict exact from="/jobs-v2" to="/jobs-v2/"/>
            <Redirect to="/home"/>
            
          </Switch>
        </React.Suspense>
      </Content>
    </DrawerProvider>
  );
}

const App: React.FC<BootstrapProps> = (props) => {
  return (
    <SnackbarProvider>
      <Contexts {...props} >
      <Helmet
        titleTemplate="Apulis Deep Learning Platform"
        defaultTitle="Apulis Deep Learning Platform"
      />
      <CssBaseline/>
      <Box display="flex" minHeight="100vh" maxWidth="100vw">
        <React.Suspense fallback={Loading}>
          <Switch>
            {/* <Route exact path="/sign-in" component={SignIn}/>
            <Route exact path="/sign-up" component={SignUp} /> */}
            {/* <Route exact path="/empty-team" component={EmptyTeam} /> */}
            <Route component={Layout}/>
          </Switch>
        </React.Suspense>
      </Box>
    </Contexts>
  </SnackbarProvider>
)};

export default App;
