const uuid = require('uuid');

/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 * @property {import('../../../services/user')} user
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster, user } = context.state;
  const { teamId } = context.params;
  const job = Object.assign({}, context.request.body);
  // job['Password'] = user.Password;
  job['userName'] = user.userName;
  job['familyToken'] = uuid();
  job['isParent'] = 1;
  job['vcName'] = teamId;

  context.body = await cluster.postInferenceJob(job);
}
