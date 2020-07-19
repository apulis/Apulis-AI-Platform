const Koa = require('koa')
const Session = require('koa-session')
const mount = require('koa-mount')

const app = module.exports = new Koa()

app.keys = ['some secret hurr...']
app.use(Session(app))

app.use(mount('/api', require('./api')))
app.use(require('./frontend-dev'))

/* istanbul ignore if */
if (require.main === module) {
  app.listen(process.env.PORT || 3081, process.env.HOST)
}
