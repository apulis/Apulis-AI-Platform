const compose = require('koa-compose')
const send = require('koa-send')
const serve = require('koa-static')

const index = (context, next) => {
  if (context.method !== 'GET') { return next() }
  if (context.accepts('html') !== 'html') { return next() }
  if (context.request.header.host === 'localhost:3081') {
    return context.redirect('http://localhost:3000')
  } 
  return send(context, 'build/index.html')
}

module.exports = compose([
  index,
  serve('build')
])

