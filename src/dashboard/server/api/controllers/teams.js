const config = require('config')
const { flatMap, groupBy, map, stubArray } = require('lodash')

const Cluster = require('../services/cluster')
const User = require('../services/user')


const clusterIds = Object.keys(config.get('clusters'))

/**
 * @param {string} json the JSON text
 * @param {any} empty empty object when failed to parse
 * @return {any} parsed JSON or the empty object
 */
const tryParseJSON = (json, empty) => {
  try {
    return JSON.parse(json)
  } catch (e) {
    return empty
  }
}

/** @type {import('koa').Middleware} */
module.exports = async context => {
  const getClusterTeams = async id => {
    const cluster = new Cluster(context, id)
    // const userTeams = await User.getUserVc(context, context.cookies.get('token'))
    const teams = await cluster.getTeams()
    let teamsData = []
    // teams.forEach(i => {
    //   if (userTeams.vcList.findIndex(m => i.vcName === m) > -1) teamsData.push(i);
    // })
    return teams.map(i => {
      // @ts-ignore
      const { vcName, admin, metadata, quota } = i;
      const metadataObject = tryParseJSON(metadata, Object.create(null))
      const quotaObject = tryParseJSON(quota, Object.create(null))
      const gpus = Object.create(null)
      let userQuota = {}
      for (const model of Object.keys(metadataObject)) {
        const perNode = metadataObject[model]['num_gpu_per_node']
        gpus[model] = { perNode }
        userQuota[model] = metadataObject[model]['user_quota']
      }
      for (const model of Object.keys(quotaObject)) {
        const quota = quotaObject[model]
        if (gpus[model]) {
          gpus[model].quota = quota
        } else {
          gpus[model] = { quota }
        }
      }
      return { id, teamId: vcName, admin, gpus, userQuota }
    })
  }

  const clusterTeams = flatMap(await Promise.all(clusterIds.map(
    id => getClusterTeams(id).catch(stubArray) // ignore error and return empty array
  )))
  const teams = map(groupBy(clusterTeams, 'teamId'), (clusters, id) => {
    clusters.forEach(cluster => { delete cluster.teamId })
    return { id, clusters }
  })
  context.body = teams
}
