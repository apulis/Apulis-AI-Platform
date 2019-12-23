/** @type {import('koa').Middleware} */
module.exports = async context => {
  const { user } = context.state
  const { nickName, userName, password } = context.query
  const data = await user.signup(nickName, userName, password)
  context.log.warn(data, 'User signup')
  if(data.result) {
    await user.getAccountInfo()
    context.cookies.set('token', user.toCookie())
    context.body = data
  } else {
    context.body = data
  }
}
