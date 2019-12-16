// Create a new instance of CASAuthentication.
const config = require('config')
const Cas = require('../modules/cas')

const domain = config.get('domain').replace(/^https/, 'http')
module.exports = new Cas({
    cas_url: config.get('casUrl'),
    service_url: `${domain}/api`,
    logout_redirect_url: `${domain}/api/authenticate/zjlab?action=logout`,
    session_name: 'cas_user',
    session_info: false
})
