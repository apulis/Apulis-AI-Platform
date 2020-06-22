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
  const jobs = await cluster.getInferenceJobs(teamId, jobOwner, Number(limit));
  jobs.sort((jobA, jobB) => {
    const jobATime = jobA['jobTime'];
    const jobBTime = jobB['jobTime'];
    const jobADate = new Date(jobATime || 0);
    const jobBDate = new Date(jobBTime || 0);
    return jobBDate - jobADate;
  })

  context.body = jobs;
}
