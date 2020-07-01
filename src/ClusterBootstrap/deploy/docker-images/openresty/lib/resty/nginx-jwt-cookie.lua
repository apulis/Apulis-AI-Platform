local jwt = require "resty.jwt"
local ck = require "resty.cookie"
local cjson = require "cjson"
--your secret
local secret = "Sign key for JWT"

local M = {}

function M.auth(claim_specs)
    -- require cookie
    local cookie, err = ck:new()
    if not cookie then
        ngx.log(ngx.ERR, err)
        ngx.exit(ngx.HTTP_UNAUTHORIZED)
    end

    -- get single cookie
    local token, err = cookie:get("token")
    if not token then
        ngx.log(ngx.ERR, err)
        ngx.exit(ngx.HTTP_UNAUTHORIZED)
    end
    -- ngx.log(ngx.INFO, "Authorization: " .. auth_header)

    -- require Bearer token
    -- local _, _, token = string.find(auth_header, "Bearer%s+(.+)")

    if token == nil then
        ngx.log(ngx.WARN, "Missing token")
        ngx.exit(ngx.HTTP_UNAUTHORIZED)
    end

    ngx.log(ngx.INFO, "Token: " .. token)

    local jwt_obj = jwt:verify(secret, token)
    if jwt_obj.verified == false then
        ngx.log(ngx.WARN, "Invalid token: ".. jwt_obj.reason)
        ngx.exit(ngx.HTTP_UNAUTHORIZED)
    end

    ngx.log(ngx.INFO, "JWT: " .. cjson.encode(jwt_obj))

end

return M