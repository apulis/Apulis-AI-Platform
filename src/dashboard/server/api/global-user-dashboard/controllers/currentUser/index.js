module.exports = async context => {
  const { user } = context.state
  const currentAuthority = []
  if (user.isAdmin) currentAuthority.push('admin')
  if (user.isAuthorized) currentAuthority.push('user')
  const body = {
    userName: user.userName,
    nickName: user.nickName,
    currentAuthority,
    uid: user.uid,
    group: user.group,
  }
  if (body.group === 'Microsoft') {
    body.openId = user.openId
  }
  context.body = body
}
