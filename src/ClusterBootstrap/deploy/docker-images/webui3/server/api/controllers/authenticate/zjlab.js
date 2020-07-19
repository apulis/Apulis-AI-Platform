const compose = require('koa-compose')
const User = require('../../services/user')
const casClient = require('../../casClient')

const login = async context => {
    const zjlabId = context.session[casClient.session_name]
    if(!zjlabId) {
      context.log.error({ zjlabId }, 'CAS failed')
      return context.redirect('/')
    }

    const user = User.fromZjlab(context, zjlabId)
    await user.getAccountInfo()
    context.cookies.set('token', user.toCookie())

    return context.redirect('/')
}

/** @type {import('koa').Middleware} */
module.exports = async context => {
  context.log.info({ query: context.query }, 'CAS succeessful callback')
  const { action } = context.query
  if(action === 'logout') {
    return context.redirect('/')
  }
  return compose([
    casClient.bounce,
    login
  ])(context)
}


