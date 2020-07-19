
module.exports = needPermission => async (context, next) => {
  if (!needPermission) {
    return next()
  }
  const user = context.state.user;
  const permissionList = user.permissionList || []
  let isAllowed = false
  if (Array.isArray(needPermission)) {
    needPermission.forEach(p => {
      if (permissionList.includes(p)) {
        isAllowed = true
      }
    })
  } else if (typeof needPermission === 'string') {
    if (permissionList.includes(needPermission)) {
      isAllowed = true
    }
  }
  if (isAllowed) {
    return next()
  } else {
    context.assert(isAllowed, 403)
  }
}
