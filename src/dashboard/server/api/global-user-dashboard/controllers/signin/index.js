const User = require('../../service/user')

module.exports = async context => {
  const { userName, password } = context.request.body
  const userInfo = await User.getAccountInfoByUserName(context, userName)
  if (userInfo && typeof userInfo === 'object' && userInfo !== null) {
    const userPassword = userInfo.password
    if (userPassword === password) {
      const user = new User(context, userInfo.openId, userInfo.group)
      context.cookies.set('token', user.toCookie())
      context.body = {
        success: true,
        token: user.toCookieToken()
      }
    } else {
      context.body = {
        success: false,
        message: 'userName or password dont match'
      }
    }
  }
}
