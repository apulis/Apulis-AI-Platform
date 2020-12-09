import jobs from './en-US/pages/jobs/jobs'
import tips from './en-US/tips'
import jobV2 from './en-US/pages/jobV2/jobV2'
import hooks from './en-US/hooks/hooks'
import components from './en-US/components/components'
import layout from './en-US/layout/layout'
import home from './en-US/pages/Home/home'
import submission from './en-US/pages/Submission/submission'
import jobsV2 from './en-US/pages/jobsV2/jobsV2'
import ClusterStatus from './en-US/ClusterStatus';
import VirtualCluster from './en-US/VirtualCluster';
import model from './en-US/model';
import version from './en-US/version';
import teams from './en-US/teams';

export default {
  tips,
  jobs,
  jobV2,
  hooks,
  components,
  layout,
  home,
  submission,
  jobsV2,
  ...ClusterStatus,
  ...VirtualCluster,
  ...model,
  ...version,
  copy: 'copy',
  teams,
}