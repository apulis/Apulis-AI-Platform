/**
 * @param {string} url
 * @return {string}
 */
const getDomain = (url) => {
  const matches = url.match(/^https?\:\/\/([^\/?#]+)(?:[\/?#]|$)/i)
  const domain = (matches && matches[0]) || ''
  if (/\/$/.test(domain)) {
    return domain
  } else {
    return domain + '/'
  }
}

exports.getDomain = getDomain