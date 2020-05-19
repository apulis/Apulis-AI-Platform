local jwt = require "resty.jwt"
local cjson = require "cjson"
--your secret
local secret = "Sign key for JWT"

local M = {}

function M.auth(claim_specs)
    -- require Authorization request header
    local auth_header = ngx.var.http_Authorization

    if auth_header == nil then
        ngx.log(ngx.WARN, "No Authorization header")
        ngx.exit(ngx.HTTP_UNAUTHORIZED)
    end

    ngx.log(ngx.INFO, "Authorization: " .. auth_header)

    -- require Bearer token
    local _, _, token = string.find(auth_header, "Bearer%s+(.+)")

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

    -- write the uid variable
    ngx.var.uid = jwt_obj.payload.sub
end

return M