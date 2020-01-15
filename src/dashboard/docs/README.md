# AI 平台文档

## 整体说明

### 一、技术栈

前端部分使用了 React + Typescript + Material，使用 nodejs 的 koa 库来充当中间层的角色，来转发/整合数据，前端的请求先发到 koa 上，然后再由 koa 整合，发到 python 写的后端上，得到返回的数据后再由 koa 整合数据，返回给前端



### 二、 如何运行

在 dashboard/config 下，应有一个 local.yaml 文件，可以运行 deploy.py webui来生成这个文件，其作用是记录一写配置信息，在代码中使用

```javascript
const config = require('config')
```

便能读取到 local.yaml 的内容，当前正在使用的内容如下

```yaml
sign: "Sign key for JWT"
winbind: "Will call /domaininfo/GetUserId with userName query to get the user's id info"
masterToken: "Access token of all users"

AddGroupLink: http://add-group/
WikiLink: http://wiki/

activeDirectory:
  tenant: "19441c6a-f224-41c8-ac36-82464c2d9b13"
  clientId: "6d93837b-d8ce-48b9-868a-39a9d843dc57"
  clientSecret: "eIHVKiG2TlYa387tssMSj?E?qVGvJi[]"

dingtalk:
  appId: "dingoap3bz8cizte9xu62e"
  appSecret: "sipRMeNixpgWQOw-sI6TFS5vdvtXozY3y75ik_Zue2KGywfSBBwV7er_8yp-7vaj"

domain: "https://apulis-dev-infra01.westus.cloudapp.azure.com"
casUrl: "http://apulis-dev-infra01.westus.cloudapp.azure.com/cas"

administrators:
  - jinlmsft@hotmail.com
  - jinli.ccs@gmail.com
  - jin.li@apulis.com

clusters:
  apulis-dev:
    restfulapi: "https://apulis-china-infra01.sigsus.cn/apis"
    title: Grafana-endpoint-of-the-cluster
    workStorage: work
    dataStorage: data
    grafana: "https://apulis-dev-infra01.westus.cloudapp.azure.com/grafana/"

```

yaml 文件可以理解成是一个没有 大括号 和 引号的 json

在配置好 local.yaml 文件后，可以本地运行 `node/server `开启 node 服务，然后运行 `yarn run frontend` 开启前端开发环境

在 src/setupProxy.js 中，会将前端的 /api 开头的请求转发到本地的 3081 端口，也就是 nodejs 服务的端口。

如果想让前端和 nodejs 服务在同一个端口运行，那么 setupProxy.js 应该这样写：

```javascript
module.exports = (app) => {
  app.use('/api', require('../server/api').callback())
}
```

这样就可以使用自己写的 nodejs 来启动前端开发环境了，不方便的地方是每次改了 nodejs 的代码都要重新去运行`yarn run frontend` 重启服务，编译较慢



## 业务逻辑流程





###  一、注册/登录

采用 OAuth 2 的登录方式，注意在开发环境下，由于 nodejs 程序监听的是另一个端口，在登录页面跳转后，需要再手动将浏览器地址换成 前端的地址（以后有机会我会优化的）

## 二、

