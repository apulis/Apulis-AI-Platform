const Ajv = require('ajv')
const config = require('config')

const ajv = new Ajv()

const schema = require('../validator/config.schema.json')

module.exports = () => {
  // const validate = ajv.compile(schema)
  // const valid = validate(config.util.toObject())
  // if (!valid) {
  //   const message = validate.errors.map(
  //     error => `${error.dataPath} ${error.message}`
  //   ).join('\n')
  //   console.log('message', validate.errors)
  //   throw Error(message)
  // }
}
