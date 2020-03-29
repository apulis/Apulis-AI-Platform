const User = require('../../service/user')

module.exports = async context => {
  const { userName, password, nickName } = context.request.body
  const group = context.state.user && context.state.user.group
  const isExist = await User.getAccountInfoByUserName(context, userName)
  if (isExist) {
    context.body = {
      success: false,
      message: `UserName ${userName} is already in use`
    }
    return
  }
  let response = await User.signupWithAccount(context, nickName, userName, password, group)
  if (response.result === true) {
    context.body = {
      success: true,
      message: 'Signup succed'
    }
  } else {
    context.body = {
      success: false,
      message: response.error
    }
  }
}