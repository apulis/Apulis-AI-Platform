import React, {useCallback} from 'react';

import {
  AppBar,
  Box,
  Button,
  Grid,
  IconButton,
  Menu,
  MenuItem,
  Tooltip,
  Typography,
  Slide,
  Toolbar,
  ListItem,
  List,
  Divider,
  ListItemText,
  Dialog,
  Snackbar,
  SnackbarContent,
  Hidden
} from '@material-ui/core';
import { createStyles, makeStyles, Theme } from '@material-ui/core/styles';
import {
  AccountBox,
  Check,
  ExitToApp,
  Group,
  MenuRounded,
  Dashboard,
  Translate
} from '@material-ui/icons';
import CloseIcon from '@material-ui/icons/Close';
import { TransitionProps } from '@material-ui/core/transitions';
import DrawerContext from './Drawer/Context';
import UserContext from '../contexts/User';
import TeamContext from '../contexts/Teams';
import { Link } from 'react-router-dom';
import _ from 'lodash';
import copy from 'clipboard-copy'
import {green,purple} from "@material-ui/core/colors";
import { SlideProps } from '@material-ui/core/Slide';
import AuthzHOC from '../components/AuthzHOC';
import axios from 'axios';
import { useTranslation } from "react-i18next";

import i18n from "i18next";

const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    headerStyle: {
      backgroundColor:theme.palette.background.paper,
    },
    leftIcon: {
      marginRight: theme.spacing(1)
    },
    success: {
      backgroundColor: green[600],
    },
    title: {
      padding: theme.spacing(1)
    },
    titleLink: {
      textDecoration: 'none',
      color:purple[50]
    },
    appBar: {
      zIndex: theme.zIndex.drawer + 1
    },
    userLabel: {
      whiteSpace:'nowrap',
      cursor: 'default'
    }

  })
);
const Transition = React.forwardRef<unknown, TransitionProps>(function Transition(props, ref) {
  return <Slide direction="up" ref={ref} {...props as SlideProps} />;
});
const OpenDrawerButton: React.FC = () => {
  const {t} = useTranslation();
  const { setOpen, open } = React.useContext(DrawerContext);
  const onClick = React.useCallback(() => setOpen(!open), [setOpen, open]);
  return (
    <Tooltip title={open ? t('layout.hide') : t('layout.show') }>
      <IconButton edge="start" color="inherit" onClick={onClick}>
        <MenuRounded />
      </IconButton>
    </Tooltip>
  );
};

let TeamMenu: React.FC;
TeamMenu = () => {
  const { teams, saveSelectedTeam, selectedTeam } = React.useContext(
    TeamContext
  );

  const [open, setOpen] = React.useState(false);

  const button = React.useRef<any>(null);

  const onButtonClick = React.useCallback(() => setOpen(true), [setOpen]);
  const onMenuClose = React.useCallback(() => setOpen(false), [setOpen]);
  const onMenuItemClick = React.useCallback(
    team => () => {
      saveSelectedTeam(team);
      setOpen(false);
    },
    [saveSelectedTeam]
  );
  const styles = useStyles({});
  return (
    <>
      <Button
        ref={button}
        variant="outlined"
        color="inherit"
        onClick={onButtonClick}
        style={{ textTransform: 'none' }}
      >
        <Group className={styles.leftIcon} />
        {selectedTeam}
      </Button>
      <Menu anchorEl={button.current} open={open} onClose={onMenuClose}>
        {_.map(teams, 'id').map((team: string) => (
          <MenuItem
            key={team}
            disabled={team === selectedTeam}
            onClick={onMenuItemClick(team)}
          >
            {team === selectedTeam ? (
              <Check className={styles.leftIcon} />
            ) : (
              <Group className={styles.leftIcon} />
            )}
            <Typography>{team}</Typography>
          </MenuItem>
        ))}
      </Menu>
    </>
  );
};

const LangMenu: React.FC = () => {
  const lng = localStorage.language || navigator.language;
  const selectedMenu = lng === 'zh-CN' ? {label: '中文', value: 'zh-CN'} : {label: 'English', value: 'en-US'}
  const menus = [{label: '中文', value: 'zh-CN'}, {label: 'English', value: 'en-US'}]

  const [open, setOpen] = React.useState(false);

  const button = React.useRef<any>(null);

  const onButtonClick = React.useCallback(() => setOpen(true), [setOpen]);
  const onMenuClose = React.useCallback(() => setOpen(false), [setOpen]);
  const changeLanguage = (lng: string) => {
    i18n.changeLanguage(lng);
  };
  const selectLang = (lang: string) => {
    localStorage.language = lang;
    changeLanguage(lang);
    document.cookie=`lang=${lang}; path=/`;
  };
  const onMenuItemClick = React.useCallback(
    menu => () => {
      selectLang(menu.value);
      setOpen(false);
    },
    [selectedMenu]
  );  
  const styles = useStyles({});
  return (
    <>
      <Button
        ref={button}
        variant="outlined"
        color="inherit"
        onClick={onButtonClick}
        style={{ textTransform: 'none' }}
      >
        <Translate className={styles.leftIcon} />
        {selectedMenu.label}
      </Button>
      <Menu anchorEl={button.current} open={open} onClose={onMenuClose}>
        {menus.map((menu: any) => (
          <MenuItem
            key={menu.value}
            disabled={menu.value === selectedMenu.value}
            onClick={onMenuItemClick(menu)}
          >
            {menu.value === selectedMenu.value ? (
              <Check className={styles.leftIcon} />
            ) : (
              <Translate className={styles.leftIcon} />
            )}
            <Typography>{menu.label}</Typography>
          </MenuItem>
        ))}
      </Menu>
    </>
  );
};

const UserButton: React.FC = () => {
  const {t} = useTranslation();
  const [openUserProfile, setOpenUserProfile] = React.useState(false);
  const [openCopyWarn, setOpenCopyWarn] = React.useState(false);
  const { nickName, userName, permissionList, currentRole, userGroupPath } = React.useContext(UserContext);
  const styles = useStyles({});
  const handleClose = () => {
    setOpenUserProfile(false);
  }
  const handleWarnClose = () => {
    setOpenCopyWarn(false);
  }
  const showUserProfile = () => {
    // setOpenUserProfile(true);
  }

  const handleCopy = useCallback((value) => {
    copy(value);
    setOpenCopyWarn(true)
  },[])
  const classes = useStyles({})
  return (
    <main>
      <Button variant="outlined" color="inherit" style={{ marginRight: '10px' }} href={userGroupPath}>
        <Dashboard className={styles.leftIcon}/>
        {t('layout.userDashboard')}
      </Button>
      <Button variant="outlined" color="inherit" onClick={showUserProfile} className={classes.userLabel}>
        <AccountBox className={styles.leftIcon}/>
        {nickName || userName}
      </Button>
      <Dialog fullScreen open={openUserProfile} onClose={handleClose} TransitionComponent={Transition}>
        <AppBar>
          <Toolbar>
            <IconButton edge="start" color="inherit" onClick={handleClose} aria-label="close">
              <CloseIcon />
            </IconButton>
            <Typography variant="h6">
              {"Account Settings"}
            </Typography>
          </Toolbar>
        </AppBar>
        <Box m={10}>
          <List>
            <ListItem button>
              <ListItemText primary="NickName" secondary={nickName || '-'}  onClick={()=>handleCopy(nickName)}/>
            </ListItem>
            <Divider />
            <ListItem button>
              <ListItemText primary="UserName" secondary={userName}  onClick={()=>handleCopy(userName)}/>
            </ListItem>
            <Divider />
            <Divider />
            <ListItem button >
              <ListItemText primary="CurrentRole" secondary={currentRole?.join(', ')}/>
            </ListItem>
            <Divider />
          </List>
        </Box>
        <Snackbar
          anchorOrigin={{ vertical: 'bottom', horizontal: 'left' }}
          open={openCopyWarn}
          autoHideDuration={500}
          onClose={handleWarnClose}
          ContentProps={{
            'aria-describedby': 'message-id',
          }}
        >
          <SnackbarContent
            className={styles.success}
            aria-describedby="client-snackbar"
            message={<span id="message-id" >Copied Success</span>}
          />
        </Snackbar>
      </Dialog>
    </main>
  );
};
const clearAuthInfo = async (userGroupPath: string) => {
  delete localStorage.token
  await axios.get('/authenticate/logout');
  window.location.href = userGroupPath + '/user/login?' + encodeURIComponent(window.location.href);
}
const SignOutButton: React.FC = () => {
  const {t} = useTranslation();
  const { userGroupPath } = React.useContext(UserContext);
  return (
    <Tooltip title={t('layout.signOut')} onClick={() => {delete localStorage.token}}>
      <IconButton edge="end" color="inherit" onClick={() => clearAuthInfo(userGroupPath || '')}>
        <ExitToApp />
      </IconButton>
    </Tooltip>
  );
};

const Title: React.FC = () => {
  const styles = useStyles({});
  return (
    <Box component="header" className={styles.title} display="flex">
      <Link to="/" className={styles.titleLink}>
        <Typography component="h1" variant="h6" align="left">
          Apulis
        </Typography>
      </Link>
    </Box>
  );
};

const DashboardAppBar: React.FC = () => {
  const styles = useStyles({});
  //const { open } = React.useContext(DrawerContext);
  return (
    <AppBar
      component="aside"
      position="fixed"
      className={styles.appBar}
    >
      <Toolbar>
        <Grid
          container
          wrap="nowrap"
          justify="flex-end"
          alignItems="center"
        >
          <Grid item>
            <OpenDrawerButton />
          </Grid>
          <Hidden xsDown>
            <Grid item xs>
              <Title />
            </Grid>
          </Hidden>
          <Grid item >
            <TeamMenu />
          </Grid>
          <Grid item style={{ marginLeft:'10px' }}>
            <UserButton />
          </Grid>
          <Grid item style={{ marginLeft:'10px' }}>
            <LangMenu />
          </Grid>
          <Grid item>
            {' '}
            <SignOutButton />
          </Grid>
        </Grid>
      </Toolbar>
    </AppBar>
  );
};

export default DashboardAppBar;
