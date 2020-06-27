/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 */


/** @type {import('koa').Middleware} */
module.exports = async context => {
  const { cluster } = context.state;
  const ret = await cluster.getModelConvertionTypes();
  context.body = ret
}
