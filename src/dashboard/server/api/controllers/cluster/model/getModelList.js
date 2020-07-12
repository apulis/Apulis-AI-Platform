/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 * @property {import('../../../services/user')} user
 */


/** @type {import('koa').Middleware} */
module.exports = async context => {
  const { cluster, user } = context.state
  const { teamId } = context.params;
  const { limit } = context.query;
  let jobOwner = context.state.user.userName;
  const jobs = await cluster.getModelList(teamId, jobOwner, Number(limit));
  jobs.sort((jobA, jobB) => {
    const jobATime = jobA['jobTime'];
    const jobBTime = jobB['jobTime'];
    const jobADate = new Date(jobATime || 0);
    const jobBDate = new Date(jobBTime || 0);
    return jobBDate - jobADate;
  })

  context.body = jobs;
}
