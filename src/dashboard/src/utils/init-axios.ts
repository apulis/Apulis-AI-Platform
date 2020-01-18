import axios, { AxiosResponse, AxiosRequestConfig, AxiosError } from 'axios'

import message from 'antd/es/message'

axios.interceptors.request.use((config: AxiosRequestConfig) => {
  return config
}, err => {
  return Promise.reject(err)
})
interface Response extends AxiosResponse {
  success?: true
}
axios.interceptors.response.use((response: Response) => {
  const { status, config: { method } } = response
  if (status === 200) {
    if (method === 'post') {
      message.success('成功添加')
    } else if (method === 'delete') {
      message.success('成功删除')
    } else if (method === 'patch') {
      message.success('成功修改')
    } else if (method === 'put') {
      message.success('成功修改')
    }
    response.success = true
  }
  return response
}, (error: AxiosError) => {
  if (error.response) {
    let status 
    if (error.response.status) {
      status = error.response.status
    }
    if (status === 401) {
      message.error('登录失效，请重新登录')
    } else if (status === 400) {
      message.error('查询参数错误，请检查输入')
    } else if (status === 403) {
      message.error('没有权限进行该项操作，请联系系统管理员')
    } else if (status && status >= 500) {
      message.error('服务器正在升级，请联系系统管理员')
    } else {
      message.error('网络连接有点儿问题')
    }
  } else {
    message.error('网络错误')
  }
  return Promise.reject(error)
})