const config = require('config')
const fetch = require('node-fetch')
const jwt = require('jsonwebtoken')


const activeDirectoryConfig = config.get('activeDirectory')
const domain = config.get('domain')

const User = require('../../../services/user')
const { basePath } = require('../../config')
const { getDomain } = require('../../utils')
const OAUTH2_URL = `https://login.microsoftonline.com/common/oauth2/v2.0`



/**
 * @param {import('koa').Context} context
 * @return {string}
 */
const getUriWithQuery = context => {
  const originalUrl = context.req.originalUrl || context.request.originalUrl || ''
  if (/localhost/.test(context.origin)) {
    let callbackRedirectURL = ''
    if (context.query.state) {
      const stateParams = new URLSearchParams(context.query.state)
      if (stateParams.has('to')) {
        callbackRedirectURL = stateParams.get('to')
      }
    } else if (context.query.to) {
      callbackRedirectURL = context.query.to
    }
    
    return `${getDomain(callbackRedirectURL)}api${basePath}/auth/microsoft`
  } else if (/127.0.0.1/.test(context.origin)) {
    return domain + `/api${basePath}/auth/microsoft`
  } else {
    return (context.origin.replace('http', 'https') + originalUrl).split('?')[0] + `/api${basePath}/auth/microsoft`
  }
}
/**
 * @param {import('koa').Context} context
 * @return {string}
 */
const getMSCallbackUri = context => {
  const originalUrl = context.req.originalUrl || context.request.originalUrl || ''
  if (/localhost/.test(context.origin)) {
    let callbackRedirectURL = ''
    const stateParams = new URLSearchParams(context.query.state)
    if (stateParams.has('to')) {
      callbackRedirectURL = stateParams.get('to')
    }
    return getDomain(callbackRedirectURL)+ `api${basePath}/auth/microsoft`
  } else if (/127.0.0.1/.test(context.origin)) {
    return domain + `/api${basePath}/auth/microsoft`
  } else {
    return (context.origin.replace('http', 'https') + originalUrl).split('?')[0]
  }
}


/**
 * @param {import('koa').Context} context
 * @return {string}
 */
const getAuthenticationUrl = context => {
  let redirect_uri = getUriWithQuery(context)
  const params = new URLSearchParams({
    client_id: activeDirectoryConfig.clientId,
    response_type: 'code',
    redirect_uri,
    response_mode: 'query',
    scope: 'openid profile email',
    state: context.querystring
  })
  return OAUTH2_URL + '/authorize?' + params
}

/**
 * @param {import('koa').Context} context
 * @return {Promise}
 */
const getDecodedIdToken = async context => {
  const { code } = context.query
  const params = new URLSearchParams({
    client_id: activeDirectoryConfig.clientId,
    scope: 'openid profile email',
    code,
    redirect_uri: getMSCallbackUri(context),
    grant_type: 'authorization_code',
    client_secret: activeDirectoryConfig.clientSecret
  })
  context.log.info({ body: params.toString() }, 'Id Token request')
  const response = await fetch(OAUTH2_URL + '/token', {
    method: 'POST',
    body: params
  })
  let data
  try {
    data = await response.json()
  } catch (error) {
    context.log.error('jsonfy microsoft response error', error.message)
  }
  context.log.info({ data }, 'Id Token response')

  context.assert(data['error'] == null, 502, data['error'])

  return jwt.decode(data['id_token'])
}

/** @type {import('koa').Middleware} */
module.exports = async context => {
  if (context.query.code != null) {
    context.log.info({ query: context.query }, 'Authentication succeessful callback')
    const idToken = await getDecodedIdToken(context)
    context.log.info(idToken, 'Id token')
    if (!idToken) {
      context.log.error({ idToken }, 'getDecodedIdToken failed')
      return context.redirect(basePath)
    }
    const user = User.fromIdToken(context, idToken)
    await user.getAccountInfo()
    context.cookies.set('token', user.toCookieToken())
    try {
      const stateParams = new URLSearchParams(context.query.state)
      if (stateParams.has('to')) {
        return context.redirect(stateParams.get('to') + '?token=' + user.toCookieToken())
      }
    } finally {}
    return context.redirect(basePath + '?token=' + user.toCookieToken())
  } else if (context.query.error != null) {
    context.log.error({ query: context.query }, 'Authentication failed callback')
    return context.redirect(`${basePath}`)
  } else {
    return context.redirect(getAuthenticationUrl(context))
  }
}
