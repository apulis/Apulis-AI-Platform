/**
 * @typedef {Object} State
 * @property {import('../../services/cluster')} cluster
 */

/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  const { cluster } = context.state
  const { teamId } = context.params
  console.log('teamId', teamId)
  const team = await cluster.getTeam(teamId)
  context.body = team
}
