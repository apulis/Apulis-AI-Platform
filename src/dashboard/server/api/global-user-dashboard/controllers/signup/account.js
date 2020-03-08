const User = require('../../service/user')

module.exports = async context => {
  const { userName, password, nickName } = context.request.body
  let response = await User.signupWithAccount(context, userName, password, nickName)
  if (response.result === true) {
    context.body = {
      success: true,
      message: 'sign up success'
    }
  } else {
    context.body = {
      success: false,
      message: 'sign up failed, please contact the administer'
    }
  }
}