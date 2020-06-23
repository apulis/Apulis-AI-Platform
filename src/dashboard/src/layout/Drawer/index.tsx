import React, { forwardRef, useContext, FC, useCallback, useEffect } from "react";
import { Link, LinkProps, matchPath, useLocation } from "react-router-dom";
import {
  Drawer,
  Theme,
  List,
  ListItem,
  ListItemText,
  Divider,
  useMediaQuery
} from "@material-ui/core";
import {
  useTheme,
} from "@material-ui/core/styles";
import { makeStyles, createStyles } from "@material-ui/core/styles";
import Context from "./Context";
import UserContext from '../../contexts/User';
import ConfigContext from "../../contexts/Config";
import AuthContext from '../../contexts/Auth';
import AuthzHOC from '../../components/AuthzHOC';
import { useTranslation } from "react-i18next";

const useStyles = makeStyles((theme: Theme) => createStyles({
  title: {
    padding: theme.spacing(1)
  },
  titleLink: {
    textDecoration: "none"
  },
  drawerHeader: {
    marginTop: 64,
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    padding: '0 8px',
    ...theme.mixins.toolbar,
    justifyContent: 'flex-end',
    minWidth: 200,
    textAlign: 'center'
  },
}));

export const ListLink = forwardRef<Link, LinkProps>(
  ({ to, ...props }, ref) => <Link ref={ref} to={to} {...props} />
);

const LinkListItem: FC<LinkProps> = ({ to, children }) => {
  const location = useLocation();
  const locationPathname = location.pathname;
  const toPathname = typeof to === "string" ? to
    : typeof to === "object" ? to.pathname
      : undefined;
  const selected = typeof toPathname === "string"
    ? matchPath(locationPathname, toPathname) !== null
    : true;
  return (
    <ListItem button selected={selected} component={ListLink} to={to}>
      {children}
    </ListItem>
  );
};

const NavigationList: FC = () => {
  const { t } = useTranslation();
  const styles = useStyles();
  return (
    <List component="nav" className={styles.drawerHeader}>
      <LinkListItem to="/home">
        <ListItemText>{t("layout.Home")}</ListItemText>
      </LinkListItem>
      <AuthzHOC needPermission={'SUBMIT_TRAINING_JOB'}>
        <LinkListItem to="/submission/training">
          <ListItemText>{t("layout.SubmitTrainingJob")}</ListItemText>
        </LinkListItem>
      </AuthzHOC>
      <AuthzHOC needPermission={['SUBMIT_TRAINING_JOB', 'VIEW_AND_MANAGE_ALL_USERS_JOB', 'VIEW_ALL_USER_JOB']}>
        <LinkListItem to="/jobs-v2">
          <ListItemText>{t("layout.ViewandManageJobs")}</ListItemText>
        </LinkListItem>
      </AuthzHOC>
      <AuthzHOC needPermission={'VIEW_CLUSTER_STATUS'}>
        <LinkListItem to="/cluster-status">
          <ListItemText>{t("layout.ClusterStatus")}</ListItemText>
        </LinkListItem>
      </AuthzHOC>
      <AuthzHOC needPermission={['VIEW_VC', 'MANAGE_VC']}>
        <LinkListItem to="/vc">  
          <ListItemText>{t("layout.VirtualCluster")}</ListItemText>
        </LinkListItem>
      </AuthzHOC>
    </List>
  );
};

const DashboardDrawer: FC = () => {
  const { open, setOpen } = useContext(Context);
  const onClose = useCallback(() => setOpen(false), [setOpen]);
  const theme = useTheme();
  const isDesktop = useMediaQuery(theme.breakpoints.up("sm"));
  const variant = isDesktop ? "persistent" : "temporary";

  useEffect(() => {
    if (isDesktop) { setOpen(true); }
  }, [isDesktop, setOpen]);
  return (
    <Drawer
      variant={variant}
      open={open}
      onClose={onClose}
    >
      <Divider />
      <NavigationList />
    </Drawer>
  );
};

export default DashboardDrawer;
