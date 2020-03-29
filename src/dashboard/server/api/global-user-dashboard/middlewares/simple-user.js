const User = require('../service/user')
/**
 * @param {boolean} force
 * @return {import('koa').Middleware}
 */
module.exports = (forceAuthenticated = true) => async (context, next) => {
  if (context.headers.authorization) {
    const { authorization } = context.headers
    try {
      const token = authorization
      const clientUserData = User.parseTokenToUserInfoWithoutVerify(token.replace(/^Bearer /, ''))
      console.log('clientUserData', clientUserData)
      context.state.user = clientUserData
    } catch (error) {
      context.log.error(error, 'Error in cookie authentication')
    }
  }

  if (forceAuthenticated) {
    context.assert(context.state.user != null, 401)
  }

  return next()
}
