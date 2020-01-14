import React from 'react';

import { BrowserRouter, Redirect, Route, RouteComponentProps, Switch } from "react-router-dom"

import 'typeface-roboto';
import 'typeface-roboto-mono';

import { Box, CssBaseline, createMuiTheme, CircularProgress } from '@material-ui/core';
import { ThemeProvider } from "@material-ui/styles";

import UserContext, { Provider as UserProvider } from "./contexts/User";
import { Provider as ClustersProvider } from "./contexts/Clusters";
import TeamsContext, { Provider as TeamProvider } from './contexts/Teams';

import AppBar from "./layout/AppBar";
import Content from "./layout/Content";
import Drawer from "./layout/Drawer";
import { Provider as DrawerProvider } from "./layout/Drawer/Context";
import { SnackbarProvider } from 'notistack';

const Home = React.lazy(() => import('./pages/Home'));
const SignIn = React.lazy(() => import('./pages/SignIn'));
const SignUp = React.lazy(() => import('./pages/SignUp'));
const EmptyTeam = React.lazy(() => import('./pages/EmptyTeam'));
const Submission = React.lazy(() => import('./pages/Submission'));
const Jobs = React.lazy(() => import('./pages/Jobs'));
const Job = React.lazy(() => import('./pages/Job'));
const ClusterStatus = React.lazy(() => import('./pages/ClusterStatus'));
const Vc = React.lazy(() => import('./pages/Vc/index.js'));
const User = React.lazy(() => import('./pages/User/index.js'));
const Access = React.lazy(() => import('./pages/Access/index.js'));

const theme = createMuiTheme();

interface BootstrapProps {
  uid?: string;
  openId?: string;
  group?: string;
  nickName?: string;
  userName?: string;
  password?: string;
  isAdmin?: boolean;
  isAuthorized?: boolean;
  _token?: any;
}

const Loading = (
  <Box flex={1} display="flex" alignItems="center" justifyContent="center">
    <CircularProgress />
  </Box>
);

const Contexts: React.FC<BootstrapProps> = ({ uid, openId, group, nickName, userName, password, isAdmin, isAuthorized, _token, children }) => (

  <BrowserRouter>
    <UserProvider  uid={uid} openId={openId} group={group} nickName={nickName} userName={userName} password={password} isAdmin={isAdmin} isAuthorized={isAuthorized} token={_token} >
      <TeamProvider addGroupLink="a" WikiLink="b">
        <ClustersProvider>
          <ThemeProvider theme={theme}>
            {children}
          </ThemeProvider>
        </ClustersProvider>
      </TeamProvider>
    </UserProvider>
  </BrowserRouter>
);

const Layout: React.FC<RouteComponentProps> = ({ location, history }) => {
  const { openId, group, userName } = React.useContext(UserContext);
  const { teams } = React.useContext(TeamsContext);

  React.useEffect(() => {
    if (openId === undefined || group === undefined) {
      history.replace('/sign-in');
    } else if(userName === undefined){
      history.replace('/sign-up');
    } else if(teams !== undefined && teams.length === 0) {
      history.replace('/empty-team');
    }
  }, [openId, group, userName, teams, history]);

  if (openId === undefined || group === undefined || userName === undefined || (teams !== undefined && teams.length === 0)) {
    return null;
  }

  return (
    <>
      <DrawerProvider>
        <Content>
          <AppBar />
          <Drawer />
          <React.Suspense fallback={Loading}>
            <Switch location={location}>
              <Route exact path="/" component={Home} />
              <Route path="/submission" component={Submission} />
              <Route path="/jobs/:cluster" component={Jobs} />
              <Route path="/jobs" component={Jobs} />
              <Route path="/job/:team/:clusterId/:jobId" component={Job} />
              <Route path="/cluster-status" component={ClusterStatus} />
              <Route path="/vc" component={Vc} />
              <Route path="/user" component={User} />
              <Route path="/access" component={Access} />
              <Redirect to="/" />
            </Switch>
          </React.Suspense>
        </Content>
      </DrawerProvider>
    </>
  );
}

const App: React.FC<BootstrapProps> = (props) => (
  <SnackbarProvider>
    <Contexts {...props}>
      <CssBaseline />
      <Box display="flex" minHeight="100vh" maxWidth="100vw">
        <React.Suspense fallback={Loading}>
          <Switch>
            <Route exact path="/sign-in" component={SignIn} />
            <Route exact path="/sign-up" component={SignUp} />
            <Route exact path="/empty-team" component={EmptyTeam} />
            <Route component={Layout} />
          </Switch>
        </React.Suspense>
      </Box>
    </Contexts>
  </SnackbarProvider>
  
);

export default App;
