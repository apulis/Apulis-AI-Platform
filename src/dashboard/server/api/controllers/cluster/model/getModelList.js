/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 * @property {import('../../../services/user')} user
 */


/** @type {import('koa').Middleware} */
module.exports = async context => {
  const { cluster, user } = context.state
  const { teamId } = context.params;
  const { limit, jobOwner } = context.query;
  context.body = await cluster.getModelList(teamId, jobOwner, Number(limit));
}
