const config = require('config')
const Cluster = require('../../service/cluster')

const clusterIds = Object.keys(config.get('clusters'))

module.exports = async context => {
  const identityName = context.params.userName
  const role = context.params.role
  const paload = {
    userName: context.state.user.userName,
    identityName,
    isAdmin: false,
    isAuthorized: false
  }
  if (role === 'Admin') {
    paload.isAdmin = true
  } else if (role === 'User') {
    paload.isAuthorized = true
  }
  const res = new Cluster(context, clusterIds[0]).editUserRole(paload)
  context.body = {
    success: true,
    message: 'ok'
  }
}
