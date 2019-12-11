const url = require('url'),
    http = require('http'),
    https = require('https'),
    parseXML = require('xml2js').parseString,
    XMLprocessors = require('xml2js/lib/processors')

/**
 * The CAS authentication types.
 * @enum {number}
 */
const AUTH_TYPE = {
    BOUNCE: 0,
    BOUNCE_REDIRECT: 1,
    BLOCK: 2
}

let _validateUri, _validate

/**
 * @typedef {Object} CAS_options
 * @property {string}  cas_url
 * @property {string}  service_url
 * @property {('1.0'|'2.0'|'3.0'|'saml1.1')} [cas_version='3.0']
 * @property {boolean} [renew=false]
 * @property {boolean} [is_dev_mode=false]
 * @property {string}  [dev_mode_user='']
 * @property {Object}  [dev_mode_info={}]
 * @property {string}  [session_name='cas_user']
 * @property {string}  [session_info=false]
 * @property {boolean} [destroy_session=false]
 */

/**
 * @param {CAS_options} options
 * @constructor
 */
module.exports = class Cas {
    constructor(options) {
        if (!options || typeof options !== 'object') {
            throw new Error(
                'CAS Authentication was not given a valid configuration object.'
            )
        }
        if (options.cas_url === undefined) {
            throw new Error('CAS Authentication requires a cas_url parameter.')
        }
        if (options.service_url === undefined) {
            throw new Error('CAS Authentication requires a service_url parameter.')
        }

        this.cas_version = options.cas_version !== undefined ? options.cas_version : '3.0'

        if (this.cas_version === '1.0') {
            _validateUri = '/validate'
            _validate = function (body, callback) {
                var lines = body.split('\n')
                if (lines[0] === 'yes' && lines.length >= 2) {
                    return callback(null, lines[1])
                } else if (lines[0] === 'no') {
                    return callback(new Error('CAS authentication failed.'))
                } else {
                    return callback(new Error('Response from CAS server was bad.'))
                }
            }
        } else if (this.cas_version === '2.0' || this.cas_version === '3.0') {
            _validateUri =
                this.cas_version === '2.0' ? '/serviceValidate' : '/p3/serviceValidate'
            _validate = function (body, callback) {
                parseXML(
                    body,
                    {
                        trim: true,
                        normalize: true,
                        explicitArray: false,
                        tagNameProcessors: [
                            XMLprocessors.normalize,
                            XMLprocessors.stripPrefix
                        ]
                    },
                    function (err, result) {
                        if (err) {
                            return callback(new Error('Response from CAS server was bad.'))
                        }
                        try {
                            var failure = result.serviceresponse.authenticationfailure
                            if (failure) {
                                return callback(
                                    new Error(
                                        'CAS authentication failed (' + failure.$.code + ').'
                                    )
                                )
                            }
                            var success = result.serviceresponse.authenticationsuccess
                            if (success) {
                                return callback(null, success.user, success.attributes)
                            } else {
                                return callback(new Error('CAS authentication failed.'))
                            }
                        } catch (err) {
                            console.log(err)
                            return callback(new Error('CAS authentication failed.'))
                        }
                    }
                )
            }
        } else if (this.cas_version === 'saml1.1') {
            _validateUri = '/samlValidate'
            _validate = function (body, callback) {
                parseXML(body, {
                    trim: true,
                    normalize: true,
                    explicitArray: false,
                    tagNameProcessors: [
                        XMLprocessors.normalize,
                        XMLprocessors.stripPrefix
                    ]
                }, (err, result) => {
                    if (err) {
                        return callback(new Error('Response from CAS server was bad.'))
                    }
                    try {
                        var samlResponse = result.envelope.body.response
                        var success = samlResponse.status.statuscode.$.Value.split(':')[1]
                        if (success !== 'Success') {
                            return callback(
                                new Error('CAS authentication failed (' + success + ').')
                            )
                        } else {
                            var attributes = {}
                            var attributesArray = samlResponse.assertion.attributestatement.attribute
                            if (!(attributesArray instanceof Array)) {
                                attributesArray = [attributesArray]
                            }
                            attributesArray.forEach(function (attr) {
                                var thisAttrValue
                                if (attr.attributevalue instanceof Array) {
                                    thisAttrValue = []
                                    attr.attributevalue.forEach(function (v) {
                                        thisAttrValue.push(v._)
                                    })
                                } else {
                                    thisAttrValue = attr.attributevalue._
                                }
                                attributes[attr.$.AttributeName] = thisAttrValue
                            })
                            return callback(
                                null,
                                samlResponse.assertion.authenticationstatement.subject.nameidentifier,
                                attributes
                            )
                        }
                    } catch (err) {
                        console.log(err)
                        return callback(new Error('CAS authentication failed.'))
                    }
                }
                )
            }
        } else {
            throw new Error(
                'The supplied CAS version ("' + this.cas_version + '") is not supported.'
            )
        }

        this.cas_url = options.cas_url
        var parsed_cas_url = url.parse(this.cas_url)
        this.request_client = parsed_cas_url.protocol === 'http:' ? http : https
        this.cas_host = parsed_cas_url.hostname
        this.cas_port = parsed_cas_url.port || (parsed_cas_url.protocol === 'http:' ? 80 : 443)
        this.cas_path = parsed_cas_url.pathname

        this.service_url = options.service_url

        this.renew = options.renew !== undefined ? !!options.renew : false

        this.is_dev_mode = options.is_dev_mode !== undefined ? !!options.is_dev_mode : false
        this.dev_mode_user = options.dev_mode_user !== undefined ? options.dev_mode_user : ''
        this.dev_mode_info = options.dev_mode_info !== undefined ? options.dev_mode_info : {}

        this.session_name = options.session_name !== undefined ? options.session_name : 'cas_user'
        this.session_info = ['2.0', '3.0', 'saml1.1'].indexOf(this.cas_version) >= 0 && options.session_info !== undefined ? options.session_info : false
        this.destroy_session = options.destroy_session !== undefined ? !!options.destroy_session : false

        // Bind the prototype routing methods to this instance of CASAuthentication.
        this.bounce = this.bounce.bind(this)
        this.bounce_redirect = this.bounce_redirect.bind(this)
        this.block = this.block.bind(this)
        this.logout = this.logout.bind(this)
    }

    /**
     * Bounces a request with CAS authentication. If the user's session is not
     * already validated with CAS, their request will be redirected to the CAS
     * login page.
     */
    async bounce(ctx, next) {
        // Handle the request with the bounce authorization type.
        await _handle.bind(this)(ctx, next, AUTH_TYPE.BOUNCE)
    }

    /**
     * Bounces a request with CAS authentication. If the user's session is not
     * already validated with CAS, their request will be redirected to the CAS
     * login page.
     */
    async bounce_redirect(ctx, next) {
        // Handle the request with the bounce authorization type.
        await _handle.bind(this)(ctx, next, AUTH_TYPE.BOUNCE_REDIRECT)
    }

    /**
     * Blocks a request with CAS authentication. If the user's session is not
     * already validated with CAS, they will receive a 401 response.
     */
    async block(ctx, next) {
        // Handle the request with the block authorization type.
        await _handle.bind(this)(ctx, next, AUTH_TYPE.BLOCK)
    }

    /**
     * Logout the currently logged in CAS user.
     */
    logout(ctx, next) {
        // Destroy the entire session if the option is set.
        if (this.destroy_session) {
            ctx.session.destroy(function (err) {
                if (err) {
                    console.log(err)
                }
            })
        }
        // Otherwise, just destroy the CAS session variables.
        else {
            delete ctx.session[this.session_name]
            if (this.session_info) {
                delete ctx.session[this.session_info]
            }
        }

        // Redirect the client to the CAS logout.
        ctx.redirect(this.cas_url + '/logout')
    }
}

/**
 * Handle a request with CAS authentication.
 */
async function _handle(ctx, next, authType) {
    // If the session has been validated with CAS, no action is required.
    if (ctx.session[this.session_name]) {
        // If this is a bounce redirect, redirect the authenticated user.
        if (authType === AUTH_TYPE.BOUNCE_REDIRECT) {
            ctx.redirect(ctx.query.redirectTo)
        }
        // Otherwise, allow them through to their request.
        else {
            await next()
        }
    }
    // If dev mode is active, set the CAS user to the specified dev user.
    else if (this.is_dev_mode) {
        ctx.session[this.session_name] = this.dev_mode_user
        ctx.session[this.session_info] = this.dev_mode_info
        await next()
    }
    // If the authentication type is BLOCK, simply send a 401 response.
    else if (authType === AUTH_TYPE.BLOCK) {
        ctx.status = 401
    }
    // If there is a CAS ticket in the query string, validate it with the CAS server.
    else if (ctx.query && ctx.query.ticket) {
        await _handleTicket.bind(this)(ctx, next)
    }
    // Otherwise, redirect the user to the CAS login.
    else {
        _login.bind(this)(ctx, next)
    }
}

/**
 * Redirects the client to the CAS login.
 */
function _login(ctx, next) {
    // Save the return URL in the session. If an explicit return URL is set as a
    // query parameter, use that. Otherwise, just use the URL from the request.
    ctx.session.cas_return_to = ctx.query.returnTo || url.parse(ctx.url).path

    // Set up the query parameters.
    var query = {
        service: this.service_url + url.parse(ctx.url).pathname,
        renew: this.renew
    }

    // Redirect to the CAS login.
    ctx.redirect(
        this.cas_url +
        url.format({
            pathname: '/login',
            query: query
        })
    )
}

const _request = function (requestOptions) {
    return new Promise((resolve, reject) => {
        const request = this.request_client.request(requestOptions, response => {
            response.setEncoding('utf8')
            var body = ''
            response.on('data', chunk => {
                return (body += chunk)
            })
            response.on('end', _ => {
                resolve(body)
            })
            response.on('error', err => {
                reject(err)
            })
        })

        request.on('error', err => {
            reject(err)
        })

        if (this.cas_version === 'saml1.1') {
            request.write(post_data)
        }

        request.end()
    })
}

/**
 * Handles the ticket generated by the CAS login requester and validates it with the CAS login acceptor.
 */
async function _handleTicket(ctx, next) {
    var requestOptions = {
        host: this.cas_host,
        port: this.cas_port
    }

    if (['1.0', '2.0', '3.0'].indexOf(this.cas_version) >= 0) {
        requestOptions.method = 'GET'
        requestOptions.path = url.format({
            pathname: this.cas_path + _validateUri,
            query: {
                service: this.service_url + url.parse(ctx.url).pathname,
                ticket: ctx.query.ticket
            }
        })
    } else if (this.cas_version === 'saml1.1') {
        var now = new Date()
        var post_data = '<?xml version="1.0" encoding="utf-8"?>\n' +
            '<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/">\n' +
            '  <SOAP-ENV:Header/>\n' +
            '  <SOAP-ENV:Body>\n' +
            '    <samlp:Request xmlns:samlp="urn:oasis:names:tc:SAML:1.0:protocol" MajorVersion="1"\n' +
            '      MinorVersion="1" RequestID="_' + req.host + '.' + now.getTime() + '"\n' +
            '      IssueInstant="' + now.toISOString() + '">\n' +
            '      <samlp:AssertionArtifact>\n' +
            '        ' + req.query.ticket + '\n' +
            '      </samlp:AssertionArtifact>\n' +
            '    </samlp:Request>\n' +
            '  </SOAP-ENV:Body>\n' +
            '</SOAP-ENV:Envelope>'

        requestOptions.method = 'POST'
        requestOptions.path = url.format({
            pathname: this.cas_path + _validateUri,
            query: {
                TARGET: this.service_url + url.parse(ctx.url).pathname,
                ticket: ''
            }
        })
        requestOptions.headers = {
            'Content-Type': 'text/xml',
            'Content-Length': Buffer.byteLength(post_data)
        }
    }

    try {
        const body = await _request.bind(this)(requestOptions)

        _validate(body, (err, user, attributes) => {
            if (err) {
                console.log(err)
                ctx.status = 401
            } else {
                ctx.session[this.session_name] = user
                if (this.session_info) {
                    ctx.session[this.session_info] = attributes || {}
                }
                ctx.redirect(ctx.session.cas_return_to)
            }
        })
    } catch (err) {
        console.log('Request error with CAS: ', err)
        ctx.status = 401
    }
}