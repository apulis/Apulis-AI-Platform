const Koa = require('koa')
const mount = require('koa-mount')

const app = module.exports = new Koa()

app.use(mount('/api', require('./api')))
app.use(require('./frontend'))

if (require.main === module) {
  app.listen(process.env.PORT || 3081, process.env.HOST)
}
