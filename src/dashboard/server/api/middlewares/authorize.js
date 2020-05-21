
module.exports = needPermission => async (context, next) => {
  if (!needPermission) {
    return next()
  }
  const user = context.state.user;
  let isAllowed = false
  if (Array.isArray(needPermission)) {
    needPermission.forEach(p => {
      if (user.permissionList.includes(p)) {
        isAllowed = true
      }
    })
  } else if (typeof needPermission === 'string') {
    if (user.permissionList.includes(needPermission)) {
      isAllowed = true
    }
  }
  if (isAllowed) {
    return next()
  } else {
    context.assert(isAllowed, 403)
  }
}
