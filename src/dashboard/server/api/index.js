const Koa = require('koa')
const Session = require('koa-session')

const app = module.exports = new Koa()

require('./configurations/logger')(app)
require('./configurations/config')(app)

var fs = require('fs');
var util = require('util');
var log_file = fs.createWriteStream(__dirname + '/debug.log', {flags : 'w'});
var log_stdout = process.stdout;



console.log = function(d) { //
  log_file.write(util.format(d) + '\n');
  log_stdout.write(util.format(d) + '\n');
};

process.on('uncaughtException', (e)=>{  
  console.log('process error is:', e.message);
})

const router = require('./router')

app.keys = ['some secret hurr...']
app.use(Session(app))

app.use(router.routes())
app.use(router.allowedMethods())

/* istanbul ignore if */
if (require.main === module) {
  app.listen(process.env.PORT || 3081, process.env.HOST)
}

console.log('1111111111111')