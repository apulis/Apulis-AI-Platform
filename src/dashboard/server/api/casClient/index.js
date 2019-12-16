// Create a new instance of CASAuthentication.
const config = require('config')
const Cas = require('../modules/cas')

module.exports = new Cas({
    cas_url: config.get('casUrl'),
    service_url: `${config.get('domain')}/api`,
    logout_redirect_url: config.get('domain'),
    session_name: 'cas_user',
    session_info: false
})
