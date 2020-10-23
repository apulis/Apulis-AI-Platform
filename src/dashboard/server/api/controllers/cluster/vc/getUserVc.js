const User = require('../../../services/user')

module.exports = async context => {
  if (context.cookies.get('token')) {
    try {
      const token = context.cookies.get('token')
      const res = await User.getUserVc(context, token)
      context.body = res
    } catch (error) {
      context.log.error(error, 'Error in cookie authentication')
    }
  }
}
