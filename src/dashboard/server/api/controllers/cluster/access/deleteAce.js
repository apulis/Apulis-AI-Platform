/**
 * @typedef {Object} State
 * @property {import('../../services/cluster')} cluster
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster, user } = context.state
  const url = context.url.split('?')[1]
  const tmp = url.split('&')
  let params = {}
  for (let item of tmp) {
    let [key, value] = item.split('=')
    params[key] = value
  }
  params.userName = user.userName
  const ret = await cluster.deleteAce(params)
  context.body = ret
}
