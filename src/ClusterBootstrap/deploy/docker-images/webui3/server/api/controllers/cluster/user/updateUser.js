/**
 * @typedef {Object} State
 * @property {import('../../services/cluster')} cluster
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster } = context.state
  const params = context.params
  const ret = await cluster.updateUserPerm(params)
  context.body = ret
}
