module.exports = async context => {
  const { user } = context.state
  context.body = user
}