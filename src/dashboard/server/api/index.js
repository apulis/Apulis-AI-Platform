const Koa = require('koa')
const Session = require('koa-session')

const app = module.exports = new Koa()

require('./configurations/logger')(app)
require('./configurations/config')(app)

const router = require('./router')

app.keys = ['some secret hurr...']
app.use(Session(app))

app.use(router.routes())
app.use(router.allowedMethods())

/* istanbul ignore if */
if (require.main === module) {
  app.listen(process.env.PORT || 3081, process.env.HOST)
}
