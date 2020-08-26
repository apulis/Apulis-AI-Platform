/**
 * @typedef {Object} State
 * @property {import('../../services/cluster')} cluster
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster, user } = context.state
  const params = context.params
  const { size, page } = context.query
  params.userName = user.userName
  params.page = page
  params.size = size
  const ret = await cluster.listVc(params)
  context.body = ret
}
