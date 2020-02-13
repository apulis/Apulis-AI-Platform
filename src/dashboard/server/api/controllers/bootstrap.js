const config = require('config')

const authEnabled = config.get('authEnabled')
/**
 * @typedef {Object} State
 * @property {import('../services/user')} user
 */

/** @type {import('koa').Middleware<State>} */
module.exports = (context) => {
  const { user } = context.state
  context.type = 'js'
  if (user) {
    context.body = `bootstrap(${JSON.stringify({ ...user, authEnabled, password: '******' })})`
  } else {
    context.body = `bootstrap(${JSON.stringify({ authEnabled })})`
  }
}
