const { createHash } = require('crypto')

const config = require('config')
const jwt = require('jsonwebtoken')
const fetch = require('node-fetch')

const Service = require('./service')
const Cluster = require('./cluster')

const sign = config.get('sign')
// const winbind = config.get('winbind')
const masterToken = config.get('masterToken')
const clusterIds = Object.keys(config.get('clusters'))

class User extends Service {
  /**
   * @param {import('koa').Context} context
   * @param {string} userName
   */
  constructor(context, userName) {
    super(context)
    this.userName = userName
  }

  /**
   * @param {import('koa').Context} context
   * @param {object} idToken
   * @return {User}
   */
  static fromIdToken(context, idToken) {
    const user = new User(context, idToken['email'])
    user.Alias = idToken['name']
    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {object} idToken
   * @return {User}
   */
  static fromDingtalk(context, userinfo) {
    const user = new User(context, userinfo['openid'])
    user.Alias = userinfo['nick']
    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {object} idToken
   * @return {User}
   */
  static fromToken(context, userName, token) {
    const user = new User(context, userName)
    const expectedToken = user.token
    const actualToken = Buffer.from(token, 'hex')
    context.assert(expectedToken.equals(actualToken), 403, 'Invalid token')

    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {string} token
   * @return {User}
   */
  static fromCookie(context, token) {
    const payload = jwt.verify(token, sign)
    const user = new User(context, payload['userName'])

    user.uid = payload['uid']
    user.gid = payload['gid']
    user.Alias = payload['Alias']
    user.groups = payload['groups']
    user.Password = payload['Password']
    user.isAdmin = payload['isAdmin']
    user.isAuthorized = payload['isAuthorized']
    return user
  }

  get token() {
    if (this._token == null) {
      const hash = createHash('md5')
      hash.update(`${this.userName}:${masterToken}`)
      this._token = hash.digest()
    }
    return this._token
  }

  async fillIdFromWinbind() {
    const params = new URLSearchParams({ userName: this.userName })
    const url = `${winbind}/domaininfo/GetUserId?${params}`
    this.context.log.info({ url }, 'Winbind request')
    const response = await fetch(url)
    const data = await response.json()
    this.context.log.info({ data }, 'Winbind response')
    
    this.uid = data['uid']
    this.gid = data['gid']
    return data
  }

  async addUserToCluster(data) {
    // Fix groups format
    if (Array.isArray(data['groups'])) {
      data['groups'] = JSON.stringify(data['groups'].map(e => String(e)))
    }
    const params = new URLSearchParams(Object.assign({ userName: this.userName }, data))
    for (const clusterId of clusterIds) {
      new Cluster(this.context, clusterId).fetch('/AddUser?' + params)
    }
  }

  async loginWithMicrosoft() {
    const params = new URLSearchParams(Object.assign({
      identityName: this.userName,
      Alias: this.Alias,
      Group: "Microsoft",
      isAdmin: true,
      isAuthorized: true
    }))
    const clusterId = clusterIds[0]
    const response = await new Cluster(this.context, clusterId).fetch('/login?' + params)
    return await response.json()
  }
  
  async loginWithDingtalk() {
    const params = new URLSearchParams(Object.assign({
      identityName: this.userName,
      Alias: this.Alias,
      Group: "DingTalk",
      isAdmin: false,
      isAuthorized: false
    }))
    const clusterId = clusterIds[0]
    const response = await new Cluster(this.context, clusterId).fetch('/login?' + params)
    return await response.json()
  }
  

  async getAccountInfo() {
    const params = new URLSearchParams(Object.assign({ identityName: this.userName }))
    const clusterId = clusterIds[0]
    const response = await new Cluster(this.context, clusterId).fetch('/getAccountInfo?' + params)
    const data = await response.json()
    this.context.log.warn(data, 'getAccountInfo')

    this.uid = data['uid']
    this.gid = data['gid']
    this.Alias = data['Alias']
    this.groups = data['groups']
    this.Password = data['Password']
    this.isAdmin = data['isAdmin']
    this.isAuthorized = data['isAuthorized']
    return data
  }

  /**
   * @return {string}
   */
  toCookie() {
    // console.log('token is ', this.token)
    return jwt.sign({
      userName: this.userName,
      uid: this.uid,
      gid: this.gid,
      Alias: this.Alias,
      groups: this.groups,
      Password: this.Password,
      isAdmin: this.isAdmin,
      isAuthorized: this.isAuthorized
    }, sign)
  }

}

module.exports = User
