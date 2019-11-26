/**
 * @typedef {Object} State
 * @property {import('../../services/cluster')} cluster
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster, user } = context.state
  const params = { userName: user.email }
  const ret = await cluster.GetACL(params)
  context.body = ret
}
