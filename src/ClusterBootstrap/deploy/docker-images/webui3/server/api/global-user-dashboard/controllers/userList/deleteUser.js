const config = require('config')
const Cluster = require('../../service/cluster')

const clusterIds = Object.keys(config.get('clusters'))

module.exports = async context => {
  const identityName = context.params.userName
  const clusterId = clusterIds[0]
  await new Cluster(context, clusterId).deleteUser({
    userName: context.state.user.userName,
    identityName
  })
  context.status = 204
  context.body = {
    success: true,
    message: 'success delete ' + identityName
  }
}
