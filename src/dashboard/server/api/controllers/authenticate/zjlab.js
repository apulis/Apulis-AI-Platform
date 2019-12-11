const compose = require('koa-compose')
const User = require('../../services/user')
const casClient = require('../../casClient')

const login = async context => {
    context.log.info({ query: context.query }, 'CAS succeessful callback')
    const zjlabId = context.session[casClient.session_name]
    if(!zjlabId) {
      context.log.error({ zjlabId }, 'CAS failed')
      return context.redirect('/')
    }

    const user = User.fromZjlab(context, `ZJ${zjlabId}`)
    await user.getAccountInfo()
    context.cookies.set('token', user.toCookie())

    return context.redirect('/')
}

/** @type {import('koa').Middleware} */
module.exports = compose([
  casClient.bounce,
  login
])
