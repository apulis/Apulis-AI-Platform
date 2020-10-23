
/** @type {import('koa').Middleware} */
module.exports = async context => {
  const config = require('config')
  context.body = {
    platformName: config.get('platformName'),
    i18n: config.get('i18n')
  }
}
