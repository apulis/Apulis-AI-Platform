const Router = require('koa-router')
const config = require('config')

const router = module.exports = new Router()
// const authEnabled = config.get('authEnabled')
router.get('/',
  require('./controllers'))

router.get('/bootstrap.js',
  require('./middlewares/user')(false),
  require('./controllers/bootstrap'))

router.get('/authenticate/logout',
  require('./controllers/authenticate/logout'))

router.param('clusterId',
  require('./middlewares/cluster'))

router.get('/teams',
  require('./middlewares/user')(),
  require('./controllers/teams'))
router.get('/teams/:teamId/clusters/:clusterId',
  require('./middlewares/user')(),
  require('./controllers/team/cluster'))
router.get('/clusters/:clusterId',
  require('./middlewares/user')(),
  require('./controllers/cluster'))

router.get('/teams/:teamId/jobs',
  require('./middlewares/user')(),
  require('./controllers/team/jobs'))
router.post('/clusters/:clusterId/jobs',
  require('./middlewares/user')(),
  require('./middlewares/authorize')('SUBMIT_TRAINING_JOB'),
  require('./middlewares/body')('job'),
  require('./controllers/cluster/jobs.post'))
router.get('/clusters/:clusterId/jobs/:jobId',
  require('./middlewares/user')(),
  require('./controllers/cluster/job'))
router.get('/clusters/:clusterId/jobs/:jobId/status',
  require('./middlewares/user')(),
  require('./controllers/cluster/job/status'))
router.put('/clusters/:clusterId/jobs/:jobId/status',
  require('./middlewares/user')(),
  require('./middlewares/manager-job')('VIEW_AND_MANAGE_ALL_USERS_JOB'),
  require('./middlewares/body')('status'),
  require('./controllers/cluster/job/status.put'))
router.put('/clusters/:clusterId/jobs/:jobId/priority',
  require('./middlewares/user')(),
  require('./middlewares/manager-job')('VIEW_AND_MANAGE_ALL_USERS_JOB'),
  require('./middlewares/body')('priority'),
  require('./controllers/cluster/job/priority.put'))
router.get('/clusters/:clusterId/jobs/:jobId/log',
  require('./middlewares/user')(),
  require('./controllers/cluster/job/log'))

router.get('/clusters/:clusterId/jobs/:jobId/commands',
  require('./middlewares/user')(),
  require('./controllers/cluster/job/commands'))
router.post('/clusters/:clusterId/jobs/:jobId/commands',
  require('./middlewares/user')(),
  require('./middlewares/body')('command'),
  require('./middlewares/manager-job')('VIEW_AND_MANAGE_ALL_USERS_JOB'),
  require('./controllers/cluster/job/commands.post'))

router.get('/clusters/:clusterId/jobs/:jobId/endpoints',
  require('./middlewares/user')(),
  require('./controllers/cluster/job/endpoints'))
router.post('/clusters/:clusterId/jobs/:jobId/endpoints',
  require('./middlewares/user')(),
  require('./middlewares/body')('endpoints'),
  require('./middlewares/authorize')('SUBMIT_TRAINING_JOB'),
  require('./controllers/cluster/job/endpoints.post'))

router.get('/user',
  require('./middlewares/user')(),
  require('./controllers/user'))

router.get('/teams/:teamId/templates',
  require('./middlewares/user')(),
  require('./controllers/team/templates'))
router.put('/teams/:teamId/templates/:templateName',
  require('./middlewares/user')(),
  require('./middlewares/body')('template'),
  require('./middlewares/authorize')('SUBMIT_TRAINING_JOB'),
  require('./controllers/team/template.put'))
router.delete('/teams/:teamId/templates/:templateName',
  require('./middlewares/user')(),
  require('./middlewares/authorize')('SUBMIT_TRAINING_JOB'),
  require('./controllers/team/template.delete'))

router.get('/:clusterId/addVc/:vcName/:quota/:metadata',
  require('./middlewares/user')(),
  require('./middlewares/authorize')('MANAGE_VC'),
  require('./controllers/cluster/vc/addVc'))
router.get('/:clusterId/deleteVc/:vcName',
  require('./middlewares/user')(),
  require('./middlewares/authorize')('MANAGE_VC'),
  require('./controllers/cluster/vc/deleteVc'))
router.get('/:clusterId/listVc',
  require('./middlewares/user')(),
  require('./controllers/cluster/vc/listVc'))
router.get('/:clusterId/updateVc/:vcName/:quota/:metadata',
  require('./middlewares/user')(),
  require('./middlewares/authorize')('MANAGE_VC'),
  require('./controllers/cluster/vc/updateVc'))
router.get('/:clusterId/countJobByStatus/:targetStatus/:vcName',
  require('./middlewares/user')(),
  require('./controllers/cluster/vc/countJobByStatus'))
router.get('/:clusterId/getAllDevice',
  require('./middlewares/user')(),
  require('./controllers/cluster/vc/getAllDevice'))

router.get('/clusters/:clusterId/teams/:teamId/inferenceJobs',
  require('./middlewares/user')(),
  require('./controllers/cluster/centralReasoning/getInferenceJobs'))
router.get('/:clusterId/getAllSupportInference',
  require('./middlewares/user')(),
  require('./controllers/cluster/centralReasoning/getAllSupportInference'))
router.post('/clusters/:clusterId/teams/:teamId/postInferenceJob',
  require('./middlewares/user')(),
  require('./middlewares/body')('postInferenceJob'),
  require('./controllers/cluster/centralReasoning/postInferenceJob'))

router.get('/:clusterId/getFDInfo',
  require('./middlewares/user')(),
  require('./controllers/cluster/model/getFDInfo'))
router.get('/:clusterId/getModelConvertionTypes',
  require('./middlewares/user')(),
  require('./controllers/cluster/model/getModelConvertionTypes'))
router.get('/:clusterId/teams/:teamId/ListModelConversionJob',
  require('./middlewares/user')(),
  require('./controllers/cluster/model/getModelList'))
router.post('/:clusterId/teams/:teamId/setFDInfo',
  require('./middlewares/user')(),
  require('./middlewares/body')('setFDInfo'),
  require('./controllers/cluster/model/setFDInfo'))
router.post('/:clusterId/pushModelToFD',
  require('./middlewares/user')(),
  require('./middlewares/body')('pushModelToFD'),
  require('./controllers/cluster/model/pushModelToFD'))
router.post('/:clusterId/teams/:teamId/postModelConversionJob',
  require('./middlewares/user')(),
  require('./middlewares/body')('postModelConversionJob'),
  require('./controllers/cluster/model/postModelConversionJob'))

// router.get('/:clusterId/listUser',
//   require('./middlewares/user')(),
//   require('./controllers/cluster/user/listUser'))
// router.get('/:clusterId/updateUserPerm/:isAdmin/:isAuthorized/:identityName',
//   require('./controllers/cluster/user/updateUser'))

// router.get('/:clusterId/GetACL',
//   require('./middlewares/user')(),
//   require('./controllers/cluster/access/GetACL'))
// router.get('/:clusterId/updateAce',
//   require('./middlewares/user')(),
//   require('./controllers/cluster/access/updateAce'))
// router.get('/:clusterId/deleteAce',
//   require('./middlewares/user')(),
//   require('./controllers/cluster/access/deleteAce'))
router.get('/error.gif',
  require('./controllers/error'))
