/**
 * @typedef {Object} State
 */
const config = require('config')
const docsUrl = config.get('docs').domain
/** @type {import('koa').Middleware<State>} */
module.exports = async context => {
  // 读取配置文件
  const res = {docsUrl: docsUrl}
  context.body = res
}
