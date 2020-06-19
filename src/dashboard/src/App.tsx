import React from 'react';
import { BrowserRouter, Redirect, Route, RouteComponentProps, Switch } from "react-router-dom"
import 'typeface-roboto';
import 'typeface-roboto-mono';
import Helmet from 'react-helmet';
import { Box, CssBaseline, createMuiTheme, CircularProgress } from '@material-ui/core';
import { ThemeProvider } from "@material-ui/styles";
import initAxios from './utils/init-axios'
import ConfigContext, { Provider as ConfigProvider } from "./contexts/Config";
import UserContext, { Provider as UserProvider } from "./contexts/User";
import { Provider as ClustersProvider } from "./contexts/Clusters";
import TeamsContext, { Provider as TeamProvider } from './contexts/Teams';
import { ConfirmProvider } from './hooks/useConfirm';
import AppBar from "./layout/AppBar";
import Content from "./layout/Content";
import Drawer from "./layout/Drawer";
import { Provider as DrawerProvider } from "./layout/Drawer/Context";
import { SnackbarProvider, useSnackbar, VariantType } from 'notistack';
import './App.less';

const Home = React.lazy(() => import('./pages/Home'));
const SignIn = React.lazy(() => import('./pages/SignIn'));
const SignUp = React.lazy(() => import('./pages/SignUp'));
const EmptyTeam = React.lazy(() => import('./pages/EmptyTeam'));
const Submission = React.lazy(() => import('./pages/Submission'));
const Jobs = React.lazy(() => import('./pages/Jobs'));
const JobV2 = React.lazy(() => import('./pages/JobV2'));
const JobsV2 = React.lazy(() => import('./pages/JobsV2'));
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
  isAdmin?: boolean;
  isAuthorized?: boolean;
  config: ConfigContext;
  user: UserContext;
  authEnabled?: {
    [props: string]: 1 | 0;
  };
  administrators?: Array<[]>
}

const Loading = (
  <Box flex={1} display="flex" alignItems="center" justifyContent="center">
    <CircularProgress />
  </Box>
);



const Contexts: React.FC<BootstrapProps> = ({ uid, openId, group, nickName, userName, isAdmin, isAuthorized, children, authEnabled, administrators }) => (
  <BrowserRouter>
    <ConfigProvider>
      <UserProvider uid={uid} openId={openId} group={group} nickName={nickName} userName={userName} isAdmin={isAdmin} isAuthorized={isAuthorized} authEnabled={authEnabled} administrators={administrators} >
          <ConfirmProvider>
            <TeamProvider>
              <ClustersProvider>
                <ThemeProvider theme={theme}>
                  {children}
                </ThemeProvider>
              </ClustersProvider>
            </TeamProvider>
          </ConfirmProvider>
      </UserProvider>
    </ConfigProvider>
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
      const redict = history.location.pathname;
      history.replace(`/empty-team?redict=${redict}`);
    }
  }, [openId, group, userName, teams, history]);
  const { enqueueSnackbar } = useSnackbar();
  initAxios((type: VariantType, msg: string) => {
    enqueueSnackbar(msg, {
      autoHideDuration: 3000,
      variant: type,
    });
  }, history);
  if (openId === undefined || group === undefined || userName === undefined || (teams !== undefined && teams.length === 0)) {
    return null;
  }

  return (
    <DrawerProvider>
      <Content>
        <AppBar />
        <Drawer />
        <React.Suspense fallback={Loading}>
          <Switch location={location}>
            <Route exact path="/" component={Home}/>
            <Route path="/submission" component={Submission}/>
            <Route path="/jobs/:cluster" component={Jobs}/>
            <Route path="/jobs" component={Jobs}/>
            <Route strict exact path="/jobs-v2/:clusterId/:jobId" component={JobV2}/>
            <Redirect strict exact from="/jobs-v2/:clusterId" to="/jobs-v2/:clusterId/"/>
            <Route strict exact path="/jobs-v2/:clusterId/" component={JobsV2}/>
            <Redirect strict exact from="/jobs-v2" to="/jobs-v2/"/>
            <Route strict exact path="/jobs-v2/" component={JobsV2}/>
            <Route path="/job/:team/:clusterId/:jobId" component={Job}/>
            <Route path="/cluster-status" component={ClusterStatus}/>
            <Route path="/vc" component={Vc} />
            <Route path="/user" component={User} />
            <Route path="/access" component={Access} />
            <Redirect to="/"/>
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
        titleTemplate="%s - Deep Learning Training Service"
        defaultTitle="Deep Learning Training Service"
      />
      <CssBaseline/>
      <Box display="flex" minHeight="100vh" maxWidth="100vw">
        <React.Suspense fallback={Loading}>
          <Switch>
            <Route exact path="/sign-in" component={SignIn}/>
            <Route exact path="/sign-up" component={SignUp} />
            <Route exact path="/empty-team" component={EmptyTeam} />
            <Route component={Layout}/>
          </Switch>
        </React.Suspense>
      </Box>
    </Contexts>
  </SnackbarProvider>
)};

export default App;
