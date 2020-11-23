/**
 * @typedef {Object} State
 * @property {import('../../services/cluster')} cluster
 * @property {import('../../services/user')} user
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster } = context.state
  const cookie = context.cookies.get('token')
  const image = Object.assign({}, context.request.body)
  context.body = await cluster.saveImage(image, cookie)
}
