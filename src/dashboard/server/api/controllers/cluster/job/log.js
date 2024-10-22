/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster } = context.state
  const { jobId } = context.params
  const { cursor, page } = context.query

  context.body = await cluster.getJobLog(jobId, page, cursor)
}
