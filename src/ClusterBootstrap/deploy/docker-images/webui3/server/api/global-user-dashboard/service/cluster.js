
const MainCluster = require('../../services/cluster')

class Cluster extends MainCluster {
  async login (userName, password) {
    const params = new URLSearchParams({
      userName,
      password
    })
    let response = await this.fetch('/signIn?' + params)
    this.context.assert(response.ok, 502)
    response = response.text()
    return response
  }
  async signUp (userName, password, nickName) {
    //
  }
}

module.exports = Cluster
