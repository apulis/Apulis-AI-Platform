const http = require('http')
const path = require('path')

const bodyParser = require('body-parser')
const express = require('express')
const ecstatic = require('ecstatic')
const history = require('connect-history-api-fallback')

const app = express()
app.use(bodyParser.json())

app.use(history())

app.use(ecstatic({ root: path.join(__dirname, './dist') }))

http.createServer(app).listen(process.argv[2] || 3386)
