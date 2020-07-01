const User = require('../services/user')
/**
 * @param {boolean} force
 * @return {import('koa').Middleware}
 */
module.exports = (forceAuthenticated = true) => async (context, next) => {
  if (context.cookies.get('token')) {
    try {
      const token = context.cookies.get('token')
      const user = context.state.user = await User.getCurrentUserFromUserDashboard(context, token)
      context.log.info(user, 'Authenticated by cookie')
    } catch (error) {
      context.log.error(error, 'Error in cookie authentication')
    }
  }

  if (forceAuthenticated) {
    context.assert(context.state.user != null, 401)
  }

  return next()
}