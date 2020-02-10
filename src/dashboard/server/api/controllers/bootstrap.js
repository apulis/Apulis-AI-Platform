/**
 * @typedef {Object} State
 * @property {import('../services/user')} user
 */

const config = require('config')

const frontendConfig = config.has('frontend')
  ? config.get('frontend')
  : Object.create(null)

/** @type {import('koa').Middleware<State>} */
module.exports = (context) => {
  console.log('1111111', context)
  const { user } = context
  const parameter = { config: frontendConfig, user }
  context.type = 'js'
  if (user) {
    context.body = `bootstrap(${JSON.stringify({ ...user, password: '******' })})`
  } else {
    context.body = `bootstrap(${JSON.stringify({ ...user })})`
  }
}
