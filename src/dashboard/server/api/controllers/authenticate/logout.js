// const config = require('config')

// const activeDirectoryConfig = config.get('activeDirectory')

// const OAUTH2_URL = `https://login.microsoftonline.com/${activeDirectoryConfig.tenant}/oauth2`

const casClient = require('../../casClient')

/** @type {import('koa').Middleware} */
module.exports = async context => {
  context.cookies.set('token')
  context.status = 200
  context.body = 'ok'
}
