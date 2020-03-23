const config = require('config')
const Cluster = require('../../service/cluster')

const clusterIds = Object.keys(config.get('clusters'))
module.exports = async context => {
  const clusterId = clusterIds[0]
  const response = await new Cluster(context, clusterId).getAllUsers()
  if (Array.isArray(response)) {
    response.forEach(r => {
      r.role = 'Unauthorized'
      if (r.isAuthorized) {
        r.role = 'User'
      }
      if (r.isAdmin) {
        r.role = 'Admin'
      }
      if (r.password) {
        delete r.password
      }
    })
  }
  context.body = {
    success: true,
    data: response
  }
}
