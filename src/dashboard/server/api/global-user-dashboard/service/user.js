const { createHash } = require('crypto')

const config = require('config')
const jwt = require('jsonwebtoken')

const Service = require('./service')
const Cluster = require('./cluster')

const clusterIds = Object.keys(config.get('clusters'))

const sign = config.get('sign')
const masterToken = config.get('masterToken')


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
  static fromWechat(context, userInfo) {
    // 对于微信，首先选择 unionId 作为 openId
    let userId = ''
    if (userInfo.unionId) {
      userId = 'unionId--' + userInfo.unionId
    } else {
      userId = 'openId--' + userInfo.openId
    }
    const user = new User(context, userId, 'Wechat')
    user.nickName = userInfo.nickname
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
    user.group = payload['group']
    user.openId = payload['openId']
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

  /**
   * @param {string} email
   * @return {Buffer}
   */
  static generateToken (email) {
    const hash = createHash('md5')
    hash.update(`${email}:${masterToken}`)
    return hash.digest()
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
      this.openId = data['openId']
      this.group = data['group']
    }
    return data
  }

  static async getAccountInfoByUserName (context, userName) {
    const params = new URLSearchParams(Object.assign({
      userName: userName,
    }))

    const clusterId = clusterIds[0]
    const response = await new Cluster(context, clusterId).fetch('/getAccountInfoByUserName?' + params)
    const data = await response.json()
    context.log.warn(data, 'getAccountInfo')
    if (data) {
      this.uid = data['uid']
      this.nickName = data['nickName']
      this.userName = data['userName']
      this.password = data['password']
      this.isAdmin = data['isAdmin']
      this.isAuthorized = data['isAuthorized']
      this.openId = data['openId']
      this.group = data['group']
    }
    return data
  }

  static async signupWithAccount(context, nickName, userName, password) {
    const params = new URLSearchParams(Object.assign({
      openId: userName,
      group: 'Account',
      nickName: nickName,
      userName: userName,
      password: password,
      isAdmin: true,
      isAuthorized: true
    }))
    const clusterId = clusterIds[0]
    const response = await new Cluster(context, clusterId).fetch('/SignUp?' + params)
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
      givenName: this.givenName,
      exp: new Date().getTime() / 1000 + 2 * 24 * 60 * 60
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
      givenName: this.givenName,
      exp: new Date().getTime() / 1000 + 2 * 24 * 60 * 60
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
