import tips from './zh-CN/tips'
import hooks from './zh-CN/hooks/hooks'
import components from './zh-CN/components/components'
import layout from './zh-CN/layout/layout'
import home from './zh-CN/pages/Home/home'
import submission from './zh-CN/pages/Submission/submission'
import jobsV2 from './zh-CN/pages/jobsV2/jobsV2'
import ClusterStatus from './zh-CN/ClusterStatus';
import VirtualCluster from './zh-CN/VirtualCluster';

export default {
  tips,
  hooks,
  components,
  layout,
  home,
  submission,
  jobsV2,
  ...ClusterStatus,
  ...VirtualCluster,
  copy:'复制'
}