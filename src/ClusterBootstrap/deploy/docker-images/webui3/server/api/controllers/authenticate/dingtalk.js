const config = require('config')
const fetch = require('node-fetch')
const CryptoJS = require('crypto-js')

const User = require('../../services/user')

const dingtalkConfig = config.get('dingtalk')
const domain = config.get('domain')
const OAUTH2_URL = `https://oapi.dingtalk.com`

/**
 * @param {import('koa').Context} context
 * @return {string}
 */
const getUriWithoutQuery = context => {
  const originalUrl = context.req.originalUrl || context.request.originalUrl || ''
  if (/localhost/.test(context.origin)) {
    return (context.origin + originalUrl).split('?')[0]
  } else if (/127.0.0.1/.test(context.origin)) {
    return domain + '/api/authenticate/dingtalk'
  } else {
    return (context.origin.replace('http', 'https') + originalUrl).split('?')[0]
  }
}

/**
 * @param {import('koa').Context} context
 * @return {string}
 */
const getAuthenticationUrl = context => {
  const params = new URLSearchParams({
    appid: dingtalkConfig.appId,
    response_type: 'code',
    redirect_uri: getUriWithoutQuery(context),
    scope: 'snsapi_login',
    state: 'qjy-dev'
  })
  return OAUTH2_URL + '/connect/qrconnect?' + params
}

const getUserinfo = async context => {
  const { code } = context.query
  const timestamp = new Date().getTime().toString()
  const signature = CryptoJS.enc.Base64.stringify(CryptoJS.HmacSHA256(timestamp, dingtalkConfig.appSecret))

  const query = new URLSearchParams({
    signature,
    timestamp,
    accessKey: dingtalkConfig.appId
  })
  const url = `${OAUTH2_URL}/sns/getuserinfo_bycode?${query}`
  const response = await fetch(url, {
    headers: { 'Content-Type': 'application/json' },
    method: 'POST',
    body: JSON.stringify({
      tmp_auth_code: code
    })
  })
  const data = await response.json()
  if (data.errcode === 0) {
    return data.user_info
  }
  return null
}

/** @type {import('koa').Middleware} */
module.exports = async context => {
  if (context.query.code != null) {
    context.log.info({ query: context.query }, 'Authentication succeessful callback')
    const userinfo = await getUserinfo(context)
    context.log.info(userinfo, 'Dingtalk Userinfo')
    if (!userinfo) {
      context.log.error({ userinfo }, 'getUserinfo failed')
      return context.redirect('/')
    }

    const user = User.fromDingtalk(context, userinfo)
    await user.getAccountInfo()
    context.cookies.set('token', user.toCookie())

    return context.redirect('/')
  } else if (context.query.error != null) {
    context.log.error({ query: context.query }, 'Authentication failed callback')
    return context.redirect('/')
  } else {
    return context.redirect(getAuthenticationUrl(context))
  }
}
