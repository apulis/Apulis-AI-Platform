/**
 * @typedef {Object} State
 * @property {import('../../../services/cluster')} cluster
 */


/** @type {import('koa').Middleware} */
module.exports = async context => {

  const { cluster } = context.state;
  const res = await cluster.getModelConvertionTypes();
  context.body = res
}
