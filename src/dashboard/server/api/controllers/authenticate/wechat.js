const OAuth = require('wechat-oauth')
const jwt = require('jsonwebtoken')
const bluebird = require('bluebird')

const User = require('../../services/user')

const config = require('config')

const OAUTH_CALLBACK = '/api/authenticate/wechat'
const domain = config.get('domain')
const { appId, appSecret } = config.get('wechat')

const wxClient = new OAuth(appId, appSecret)

const getAccessToken = bluebird.promisify(wxClient.getAccessToken.bind(wxClient))
const getUser = bluebird.promisify(wxClient.getUser.bind(wxClient))

const getRedirectUrl = context => {
  const originalUrl = context.req.originalUrl || context.request.originalUrl || ''
  const { origin } = context
  let redirectUrl = ''
  if (/127.0.0.1/.test(origin)) {
    redirectUrl = domain + OAUTH_CALLBACK
  } else if (/localhost/.test(origin)) {
    redirectUrl = (origin + OAUTH_CALLBACK).split('?')[0]
  } else {
    redirectUrl = (origin.replace('http', 'https') + originalUrl).split('?')[0]
  }
  const OAUTH_URL = `https://open.weixin.qq.com/connect/qrconnect?appid=${appId}&redirect_uri=${redirectUrl}&response_type=code&scope=snsapi_userinfo,snsapi_login&state=`
  return OAUTH_URL
}

/** @type {import('koa').Middleware} */
module.exports = async context => {
  if (context.query.code) {
    const { code } = context.query
    const accessToken = await getAccessToken(code)
    const openId = accessToken.data.openid
    const unionId = accessToken.data.unionid
    const { nickname } = await getUser(openId)
    const userInfo = { nickname, openId, unionId }
    const user = User.fromWechat(context, userInfo)
    context.cookies.set('token', user.toCookieToken())
    return context.redirect('/')
  } else if (context.query.error) {
    context.log.error({ error: context.query.error }, 'wx oauth failed')
    return context.redirect('/')
  } else {
    context.redirect(getRedirectUrl(context))
  }
}
