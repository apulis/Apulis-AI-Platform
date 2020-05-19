// module.exports = (app) => {
//   app.use('/api', require('../server/api').callback())
// }


const proxy = require('http-proxy-middleware')

module.exports = (app) => {
  app.use(proxy('/api', { target: 'http://localhost:3081', changeOrigin: true }))
}
