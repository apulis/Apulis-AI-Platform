
module.exports = (needPermission) => async (context, next) => {
  const { jobId } = context.params
  const { cluster } = context.state
  const jobOwner = await cluster.getJobOwner(jobId)
  const user = context.state.user
  const { userName } = user
  const permissionList = user.permissionList || []

  let isAllowed = userName === jobOwner
  
  if (isAllowed) {
    return next()
  } else {
    if (permissionList.includes(needPermission)) {
      return next()
    }
  }
  context.assert(isAllowed, 403)
}
