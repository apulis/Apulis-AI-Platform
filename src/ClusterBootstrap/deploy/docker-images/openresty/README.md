# lua-resty-jwt
## support
current support:
    1. jwt token from cookie
    2. jwt from header

### jwt from header 
```shell script
# modify nginx.conf
local jwt = require("resty.nginx-jwt-header")
```

### jwt from cookie 
```shell script
# modify nginx.conf
local jwt = require("resty.nginx-jwt-cookie")
```
curl example

```shell script
curl 127.0.0.1:8080 --cookie "token=xxx"
```