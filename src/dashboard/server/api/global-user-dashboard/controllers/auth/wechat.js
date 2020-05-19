const OAuth = require('wechat-oauth')
const bluebird = require('bluebird')

const User = require('../../../services/user')

const config = require('config')

const domain = config.get('domain')
const { appId, appSecret } = config.get('wechat')

const { basePath } = require('../../config')
const OAUTH_CALLBACK = `/api${basePath}/authenticate/wechat`
const { getDomain } = require('../../utils')

const wxClient = new OAuth(appId, appSecret)

const getAccessToken = bluebird.promisify(wxClient.getAccessToken.bind(wxClient))
const getUser = bluebird.promisify(wxClient.getUser.bind(wxClient))

const getRedirectUrl = context => {
  const originalUrl = context.req.originalUrl || context.request.originalUrl || ''
  const { origin } = context
  let state = ''
  let env = ''
  if (context.query.to) {
    state = context.query.to
  }
  if (context.query.env) {
    env = context.query.env
  }
  console.log('env', env)
  let redirectUrl = ''
  if (env === 'dev') {
    redirectUrl = `${getDomain(state)}api${basePath}/auth/wechat`
  } else {
    redirectUrl = domain + `/api${basePath}/auth/wechat`
  }
  console.log('redirectUrl', redirectUrl, state)
  const OAUTH_URL = `https://open.weixin.qq.com/connect/qrconnect?appid=${appId}&redirect_uri=${redirectUrl}&response_type=code&scope=snsapi_userinfo,snsapi_login&state=${state}`
  return OAUTH_URL
}

/** @type {import('koa').Middleware} */
module.exports = async context => {
  if (context.query.code) {
    let cookieToken = ''
    try {
      const { code, to } = context.query
      const accessToken = await getAccessToken(code)
      const openId = accessToken.data.openid
      const unionId = accessToken.data.unionid
      const { nickname } = await getUser(openId)
      const userInfo = { nickname, openId, unionId }
      const user = User.fromWechat(context, userInfo)
      await user.getAccountInfo()
      cookieToken = user.toCookieToken()
      try {
        const stateParams = new URLSearchParams(context.query.state)
        if (stateParams.has('to')) {
          return context.redirect(stateParams.get('to') + '?token=' + cookieToken)
        }
      } finally {}
      context.cookies.set('token', cookieToken)
    } catch (error) {
      context.log.error({ error: error }, 'wx getUserInfo failed')
    }
    if (cookieToken) {
      return context.redirect(basePath + '/?token=' + cookieToken)
    }
  } else if (context.query.error) {
    context.log.error({ error: context.query.error }, 'wx oauth failed')
    return context.redirect(basePath)
  } else {
    context.redirect(getRedirectUrl(context))
  }
}
