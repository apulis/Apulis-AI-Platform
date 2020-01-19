# 注册/登录/鉴权相关接口

## 登录回调接口

路径：/api/authenticate

请求这个地址后，后端进行一次重定向到第三方的登录地址，用户在第三方网页完成登录操作，会携带一个 code 再重定向到nodejs的 url，nodejs 根据这个 code，去向第三方换取用户的信息。然后设置 cookie，再次重定向到前端的 url 上



## 用户信息接口

路径：/api/bootstrap.js

注意这是一个类似 jsonp 获取信息的方式，会将用户信息以这样的形式返回

```javascript
bootstap({
    openId: "xianjie.han@apulis.com",
    group: "Microsoft",
    uid: 30001,
    nickName: "hanxianjie_1",
    userName: "xianjie.han",
    password: "123456",
    isAdmin: 1,
    isAuthorized: 1
})
```

如果未登录，则返回的数据是 undefined

## teams 接口

路径: /api/teams

表示用户当前有权限的 virtual cluster，是一个数组，数据格式如下：

```
[{
    "id": "platform2",
    "clusters": [{
        "id": "apulis-dev",
        "admin": true,
        "gpus": {
            "None": {
                "perNode": 0,
                "quota": 0
            }
        }
    }]
}, {
    "id": "hanx",
    "clusters": [{
        "id": "apulis-dev",
        "admin": true,
        "gpus": {
            "None": {
                "perNode": 0,
                "quota": 0
            }
        }
    }]
}]

```

只有这个数组的长度大于等于 1 的时候，用户才可以进入其他页面

restful api 路径： /ListVCs?userName={userName}

在请求到数据后， nodejs 会进行一些数据格式的转换工作