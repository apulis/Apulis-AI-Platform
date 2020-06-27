const uuid = require('uuid');

/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 * @property {import('../../../services/user')} user
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster, user } = context.state;
  // const { teamId } = context.params;
  // const job = Object.assign({}, context.request.body);

  context.body = await cluster.pushModelToFD(context.request.body);
}
