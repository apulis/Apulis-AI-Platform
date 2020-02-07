const { createHash } = require('crypto')

const config = require('config')
const jwt = require('jsonwebtoken')

const Service = require('./service')
const Cluster = require('./cluster')

const clusterIds = Object.keys(config.get('clusters'))

const sign = config.get('sign')
// const winbind = config.get('winbind')
const winbind = config.has('winbind') ? config.get('winbind') : undefined
const masterToken = config.get('masterToken')

const TOKEN = Symbol('token')

class User extends Service {
  /**
   * @param {import('koa').Context} context
   * @param {string} openId
   * @param {string} group
   */
  constructor(context, openId, group) {
    super(context)
    this.openId = openId
    this.group = group
  }

  /**
   * @param {import('koa').Context} context
   * @param {object} idToken
   * @return {User}
   */
  static fromIdToken(context, idToken) {
    const user = new User(context, idToken['email'], 'Microsoft')
    user.nickName = idToken['name']
    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {object} userinfo
   * @return {User}
   */
  static fromDingtalk(context, userinfo) {
    const user = new User(context, userinfo['openid'], 'DingTalk')
    user.nickName = userinfo['nick']
    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {String} openId
   * @return {User}
   */
  static fromZjlab(context, zjlabId) {
    const user = new User(context, `ZJ${zjlabId}`, 'Zjlab')
    user.nickName = `User ${zjlabId}`
    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {string} email
   * @param {string} password
   * @return {User}
   */
  static fromToken(context, openId, group, token) {
    const user = new User(context, openId, group)
    const expectedToken = user.token
    const actualToken = Buffer.from(password, 'hex')
    context.assert(expectedToken.equals(actualToken), 403, 'Invalid token')

    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {string} cookieToken
   * @return {User}
   */
  static fromCookie(context, token) {
    const payload = jwt.verify(token, sign)
    const user = new User(context, payload['openId'], payload['group'])
    return user
  }

  /**
   * @param {import('koa').Context} context
   * @param {string} cookieToken
   * @return {User}
   */
  static fromCookieToken (context, cookieToken) {
    const payload = jwt.verify(cookieToken, sign)
    const user = new User(context, payload['email'])
    user.givenName = payload['givenName']
    user.familyName = payload['familyName']
    user.uid = payload['uid']
    user.nickName = payload['nickName']
    user.userName = payload['userName']
    user.password = payload['password']
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

  async fillIdFromWinbind () {
    if (winbind == null) {
      this.context.log.warn('No winbind server, user will have no uid / gid, and will not sync user info to any cluster.')
      return null
    }

    const params = new URLSearchParams({ userName: this.email })
    const url = `${winbind}/domaininfo/GetUserId?${params}`
    this.context.log.info({ url }, 'Winbind request')
    const response = await fetch(url)
    const data = await response.json()
    this.context.log.info({ data }, 'Winbind response')

    this.uid = data['uid']
    this.gid = data['gid']
    return data
  }

  async addUserToCluster (data) {
    if (data == null) return

    // Fix groups format
    if (Array.isArray(data['groups'])) {
      data['groups'] = JSON.stringify(data['groups'].map(e => String(e)))
    }
    const params = new URLSearchParams(Object.assign({ userName: this.userName }, data))
    for (const clusterId of clusterIds) {
      new Cluster(this.context, clusterId).fetch('/AddUser?' + params)
    }
  }
  /**
   * @param {string} email
   * @return {Buffer}
   */
  static generateToken (email) {
    const hash = createHash('md5')
    hash.update(`${email}:${masterToken}`)
    return hash.digest()
  }

  /*
  get token () {
    if (this[TOKEN] == null) {
      this[TOKEN] = User.generateToken(this.email)
    }
    return this[TOKEN]
  }*/

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
    const params = new URLSearchParams(Object.assign({
      openId: this.openId,
      group: this.group,
    }))

    const clusterId = clusterIds[0]
    const response = await new Cluster(this.context, clusterId).fetch('/getAccountInfo?' + params)
    const data = await response.json()
    this.context.log.warn(data, 'getAccountInfo')
    if (data) {
      this.uid = data['uid']
      this.nickName = data['nickName']
      this.userName = data['userName']
      this.password = data['password']
      this.isAdmin = data['isAdmin']
      this.isAuthorized = data['isAuthorized']
    }
    return data
  }

  async signup(nickName, userName, password ) {
    const params = new URLSearchParams(Object.assign({
      openId: this.openId,
      group: this.group,
      nickName: nickName,
      userName: userName,
      password: password,
      isAdmin: true,
      isAuthorized: true
    }))
    const clusterId = clusterIds[0]
    const response = await new Cluster(this.context, clusterId).fetch('/SignUp?' + params)
    return await response.json()
  }

  /**
   * @return {string}
   */
  toCookie() {
    // console.log('token is ', this.token)
    return jwt.sign({
      openId: this.openId,
      group: this.group,
      uid: this.uid,
      nickName: this.nickName,
      userName: this.userName,
      password: this.password,
      isAdmin: this.isAdmin,
      isAuthorized: this.isAuthorized,
      gid: this.gid,
      familyName: this.familyName,
      givenName: this.givenName
    }, sign)

  }
  toCookieToken () {
    return jwt.sign({
      openId: this.openId,
      group: this.group,
      uid: this.uid,
      nickName: this.nickName,
      userName: this.userName,
      password: this.password,
      isAdmin: this.isAdmin,
      isAuthorized: this.isAuthorized,
      gid: this.gid,
      familyName: this.familyName,
      givenName: this.givenName
    }, sign)
  }

  toJSON () {
    return {
      email: this.email,
      password: this.token.toString('hex'),
      uid: this.uid,
      gid: this.gid,
      familyName: this.familyName,
      givenName: this.givenName
    }
  }
}

module.exports = User
