const User = require('../services/user')
/**
 * @param {boolean} force
 * @return {import('koa').Middleware}
 */
module.exports = (forceAuthenticated = true) => async (context, next) => {
  if ('userName' in context.query && 'token' in context.query) {
    const { userName, token } = context.query
    const user = context.state.user = User.fromToken(context, userName, token)
    await user.getAccountInfo()
    context.log.warn(user, 'Authenticated by token') 
  }
  if ('email' in context.query) {
    let { email, password } = context.query

    // Backward compatibility
    if (password === undefined) { password = context.query.token }

    if (password) {
      const user = context.state.user = User.fromPassword(context, email, password)
      context.log.debug(user, 'Authenticated by password')
    }
  } else if (context.cookies.get('token')) {
    try {
      const token = context.cookies.get('token')
      const user = context.state.user = User.fromCookieToken(context, token)
      await user.password
      await user.addGroupLink
      await user.WikiLink
      context.log.info(user, 'Authenticated by cookie')
      context.user = user
    } catch (error) {
      context.log.error(error, 'Error in cookie authentication')
    }
  }

  if (forceAuthenticated) {
    context.assert(context.state.user != null, 403)
  }

  return next()
}
