local error = error
local tostring = tostring
local type = type
local ceil = math.ceil
local floor = math.floor
local ffi = require "ffi"
local ffi_new = ffi.new
local ffi_string = ffi.string


local _M = { version = "1.3.0"}


local get_string_buf
do
    local str_buf_size = 4096
    local str_buf
    local c_buf_type = ffi.typeof("char[?]")

    function get_string_buf(size)
        if size > str_buf_size then
            return ffi_new(c_buf_type, size)
        end

        if not str_buf then
            str_buf = ffi_new(c_buf_type, str_buf_size)
        end

        return str_buf
    end
end

local function load_shared_lib(so_name)
    local tried_paths = {}
    local i = 1

    for k, _ in package.cpath:gmatch("[^;]+") do
        local fpath = k:match("(.*/)")
        fpath = fpath .. so_name
        local f = io.open(fpath)
        if f ~= nil then
            io.close(f)
            return ffi.load(fpath)
        end
        tried_paths[i] = fpath
        i = i + 1
    end

    tried_paths[#tried_paths + 1] =
        'tried above paths but can not load ' .. so_name
    error(table.concat(tried_paths, '\n'))
end
local encoding = load_shared_lib("librestybaseencoding.so")


ffi.cdef([[
size_t modp_b64_encode(char* dest, const char* str, size_t len,
    uint32_t no_padding);
size_t modp_b64_decode(char* dest, const char* src, size_t len);
size_t modp_b64w_encode(char* dest, const char* str, size_t len);
size_t modp_b64w_decode(char* dest, const char* src, size_t len);
size_t b32_encode(char* dest, const char* src, size_t len, uint32_t no_padding,
    uint32_t hex);
size_t b32_decode(char* dest, const char* src, size_t len, uint32_t hex);
size_t modp_b16_encode(char* dest, const char* str, size_t len,
    uint32_t out_in_lowercase);
size_t modp_b16_decode(char* dest, const char* src, size_t len);
size_t modp_b2_encode(char* dest, const char* str, size_t len);
size_t modp_b2_decode(char* dest, const char* str, size_t len);
size_t modp_b85_encode(char* dest, const char* str, size_t len);
size_t modp_b85_decode(char* dest, const char* str, size_t len);
]])


local function check_encode_str(s)
    if type(s) ~= 'string' then
        if not s then
            s = ''
        else
            s = tostring(s)
        end
    end

    return s
end


local function base64_encoded_length(len, no_padding)
    return no_padding and floor((len * 8 + 5) / 6) or
           floor((len + 2) / 3) * 4
end


function _M.encode_base64(s, no_padding)
    s = check_encode_str(s)

    local slen = #s
    local no_padding_bool = false
    local no_padding_int  = 0

    if no_padding then
        if no_padding ~= true then
            error("boolean argument only")
        end

        no_padding_bool = true
        no_padding_int  = 1
    end

    local dlen = base64_encoded_length(slen, no_padding_bool)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b64_encode(dst, s, slen, no_padding_int)
    return ffi_string(dst, r_dlen)
end


function _M.encode_base64url(s)
    if type(s) ~= "string" then
        return nil, "must provide a string"
    end

    local slen = #s
    local dlen = base64_encoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b64w_encode(dst, s, slen)
    return ffi_string(dst, r_dlen)
end


local function check_decode_str(s, level)
    if type(s) ~= 'string' then
        error("string argument only", level + 2)
    end
end


local function base64_decoded_length(len)
    return floor((len + 3) / 4) * 3
end


function _M.decode_base64(s)
    check_decode_str(s, 1)

    local slen = #s
    local dlen = base64_decoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b64_decode(dst, s, slen)
    if r_dlen == -1 then
        return nil
    end
    return ffi_string(dst, r_dlen)
end


function _M.decode_base64url(s)
    if type(s) ~= "string" then
        return nil, "must provide a string"
    end

    local slen = #s
    local dlen = base64_decoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b64w_decode(dst, s, slen)
    if r_dlen == -1 then
        return nil, "invalid input"
    end
    return ffi_string(dst, r_dlen)
end


local function base32_encoded_length(len)
    return floor((len + 4) / 5) * 8
end


local function encode_base32(s, no_padding, hex)
    s = check_encode_str(s)

    local slen = #s
    local no_padding_int = no_padding and 1 or 0
    local dlen = base32_encoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.b32_encode(dst, s, slen, no_padding_int, hex)
    return ffi_string(dst, r_dlen)
end

function _M.encode_base32(s, no_padding)
    return encode_base32(s, no_padding, 0)
end

function _M.encode_base32hex(s, no_padding)
    return encode_base32(s, no_padding, 1)
end


local function base32_decoded_length(len)
    return floor(len * 5 / 8)
end


local function decode_base32(s, hex)
    check_decode_str(s, 1)

    local slen = #s
    if slen == 0 then
        return ""
    end

    local dlen = base32_decoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.b32_decode(dst, s, slen, hex)
    if r_dlen == -1 then
        return nil, "invalid input"
    end
    return ffi_string(dst, r_dlen)
end

function _M.decode_base32(s)
    return decode_base32(s, 0)
end

function _M.decode_base32hex(s)
    return decode_base32(s, 1)
end


local function base16_encoded_length(len)
    return len * 2
end


function _M.encode_base16(s, out_in_lowercase)
    s = check_encode_str(s)

    local out_in_lowercase_int = out_in_lowercase and 1 or 0
    local slen = #s
    local dlen = base16_encoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b16_encode(dst, s, slen, out_in_lowercase_int)
    return ffi_string(dst, r_dlen)
end


local function base16_decoded_length(len)
    return len / 2
end


function _M.decode_base16(s)
    check_decode_str(s, 1)

    local slen = #s
    if slen == 0 then
        return ""
    end

    local dlen = base16_decoded_length(slen)
    if floor(dlen) ~= dlen then
        return nil, "invalid input"
    end

    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b16_decode(dst, s, slen)
    if r_dlen == -1 then
        return nil, "invalid input"
    end
    return ffi_string(dst, r_dlen)
end


local function base2_encoded_length(len)
    return len * 8
end


function _M.encode_base2(s)
    s = check_encode_str(s)

    local slen = #s
    local dlen = base2_encoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b2_encode(dst, s, slen)
    return ffi_string(dst, r_dlen)
end


local function base2_decoded_length(len)
    return len / 8
end


function _M.decode_base2(s)
    check_decode_str(s, 1)

    local slen = #s
    if slen == 0 then
        return ""
    end

    local dlen = base2_decoded_length(slen)
    if floor(dlen) ~= dlen then
        return nil, "invalid input"
    end

    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b2_decode(dst, s, slen)
    if r_dlen == -1 then
        return nil, "invalid input"
    end
    return ffi_string(dst, r_dlen)
end


local function base85_encoded_length(len)
    return len / 4 * 5
end


function _M.encode_base85(s)
    s = check_encode_str(s)

    local slen = #s
    if slen == 0 then
        return ""
    end

    local dlen = base85_encoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b85_encode(dst, s, slen)
    return ffi_string(dst, r_dlen)
end


local function base85_decoded_length(len)
    return ceil(len / 5) * 4
end


function _M.decode_base85(s)
    check_decode_str(s, 1)

    local slen = #s
    if slen == 0 then
        return ""
    end

    local dlen = base85_decoded_length(slen)
    local dst = get_string_buf(dlen)
    local r_dlen = encoding.modp_b85_decode(dst, s, slen)
    if r_dlen == -1 then
        return nil, "invalid input"
    end
    return ffi_string(dst, r_dlen)
end


return _M