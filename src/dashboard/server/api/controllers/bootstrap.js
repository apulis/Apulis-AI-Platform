const config = require('config')
const administrators = config.get('administrators')
const authEnabled = config.get('authEnabled')
/**
 * @typedef {Object} State
 * @property {import('../services/user')} user
 */

/** @type {import('koa').Middleware<State>} */
module.exports = (context) => {
  let { user } = context.state
  context.type = 'js'
  if (user) {
    if (user.password) {
      delete user.password
    }
    context.body = `bootstrap(${JSON.stringify({ ...user, authEnabled, administrators })})`
  } else {
    context.body = `bootstrap(${JSON.stringify({ authEnabled })})`
  }
}
