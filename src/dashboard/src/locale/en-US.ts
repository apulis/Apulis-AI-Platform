import tips from './en-US/tips'
import components from './en-US/components/components'
import layout from './en-US/layout/layout'
import home from './en-US/pages/Home/home'
import submission from './en-US/pages/Submission/submission'
import jobsV2 from './en-US/pages/jobsV2/jobsV2'
import ClusterStatus from './en-US/ClusterStatus';
import VirtualCluster from './en-US/VirtualCluster';

export default {
  tips,
  components,
  layout,
  home,
  submission,
  jobsV2,
  ...ClusterStatus,
  ...VirtualCluster,
  copy:'copy'
}