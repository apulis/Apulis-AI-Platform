const Router = require('koa-router')

const router = module.exports = new Router()

// router.get('/', require('./controllers'))
router.param('clusterId',
  require('./middlewares/cluster'))
router.get('/currentUser',
  require('./middlewares/user')(),
  require('./controllers/currentUser'))
router.post('/signIn',
  require('./middlewares/body')('signIn'),
  require('./controllers/signin/index'))
router.post('/signUp',
  require('./middlewares/body')('signUp'),
  require('./controllers/signup/account'))
router.get('/userList',
  require('./middlewares/user')(),
  require('./controllers/userList'))
router.delete('/user/:userName',
  require('./middlewares/user')(),
  require('./controllers/userList/deleteUser')
)
router.patch('/userRole/:userName/role/:role',
  require('./middlewares/user')(),
  require('./controllers/userList/editUserRole')
)
router.post('/user',
  require('./middlewares/user')(),
  require('./middlewares/body')('addUser'),
  require('./controllers/userList/addUser')
)