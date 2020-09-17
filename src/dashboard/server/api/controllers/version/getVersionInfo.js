/**
 * @typedef {Object} State
 * @property {import('../../services/version')} version
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster } = context.state
  const res = await cluster.getVersionInfo()
  context.body = res
}
