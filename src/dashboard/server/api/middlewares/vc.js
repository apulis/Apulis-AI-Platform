
/**
 * @return {import('koa').Middleware}
 */
module.exports = async (context, next) => {
  const { teamId } = context.params
  const currentVC = context.state.user.currentVC
  let allowed = false
  if (currentVC.includes(teamId) || typeof teamId === 'undefined') {
    allowed = true
    return next()
  }
  context.assert(allowed, 403)
}
