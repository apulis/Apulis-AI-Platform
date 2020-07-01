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
      const clientUserData = User.parseTokenToUserInfo(token.replace(/^Bearer /, ''))
      const isSignuped = typeof clientUserData.userName === 'undefined' ? false : true
      if (isSignuped) {
        // 验证用户是否被删除
        const serverUserData = await User.getAccountInfoByUserName(context, clientUserData.userName)
        if (serverUserData) {
          context.state.user = serverUserData
        }
      } else {
        context.state.user = clientUserData;
      }
      
    } catch (error) {
      context.log.error(error, 'Error in cookie authentication')
    }
  }

  if (forceAuthenticated) {
    context.assert(context.state.user != null, 401)
  }

  return next()
}
