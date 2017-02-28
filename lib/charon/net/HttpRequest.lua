local Class      = require 'charon.oop.Class'
local url        = require 'charon.net.url'
local cookie     = require 'charon.net.cookie'
local multipart  = require 'charon.net.multi-part'
local json       = require 'charon.json'

--local HttpParser = require 'charon.net.HttpParser'

HttpRequest = HttpRequest or {}
HttpRequest.converter = require('charon.Converter')

HttpRequest.params = function(rebuild)
  if HttpRequest.__params == nil or rebuild then
    if HttpRequest.requestMethod() == 'POST' then
      if HttpRequest.field('Content-Type'):startsWith('multipart/form-data;') then
        HttpRequest.__params = multipart.parse(HttpRequest.headerDone())
      else
        HttpRequest.__params = url.parseQuery(HttpRequest.headerDone())
      end
    else
      HttpRequest.__params = url.parseQuery(HttpRequest.queryString())
    end
  end
  return HttpRequest.converter.new(HttpRequest.__params)
end

HttpRequest.cookies = function()
  if HttpRequest.__cookies == nil then
    HttpRequest.__cookies = cookie.parse(HttpRequest.field("Cookie"))
  end
  return HttpRequest.__cookies
end

HttpRequest.cookie = function(name, value, params)

  local cookie = "Set-Cookie: " .. name .. "=" .. value

  if type(params) == 'table' then

    if params.httponly then
      cookie = cookie .. '; HttpOnly'
    end

    if params.secure then
      cookie = cookie .. '; Secure'
    end

    if params.expires then
      cookie = cookie .. '; Expires=' .. params.expires
    end

    if params.domain then
      cookie = cookie .. '; Domain=' .. params.domain
    end

    if params.path then
      cookie = cookie .. '; Path=' .. params.path
    end
  end

  HttpRequest.append(cookie)
end

HttpRequest.session = function()
  local cookies = HttpRequest.cookies()
  if cookies.charon_session_id then
    if HttpRequest.__session_data == nil then
      HttpRequest.__session_id = cookies.charon_session_id
      if cache.value(HttpRequest.__session_id) then
        local data = cache.value(HttpRequest.__session_id)
        if #data == 0 then
          HttpRequest.__session_data = {}
        else
          HttpRequest.__session_data = json.decode(data)
        end
      end
    end
  else
    local uuid = os.uuid()
    HttpRequest.cookie('charon_session_id', uuid)
    HttpRequest.__session_id   = uuid
    HttpRequest.__session_data = {}
  end

  return HttpRequest.__session_data
end

HttpRequest.reset = function()
  if HttpRequest.__session_id and type(HttpRequest.__session_data) == 'table' then
    cache.insert(HttpRequest.__session_id, json.encode(HttpRequest.__session_data))
  end
  HttpRequest.__params       = nil
  HttpRequest.__response     = nil
  HttpRequest.__cookies      = nil
  HttpRequest.__session_id   = nil
  HttpRequest.__session_data = nil
end

HttpRequest.append = function(header)
  if HttpRequest.__response == nil then
    HttpRequest.__response = {}
  end
  table.insert(HttpRequest.__response, header)
end

HttpRequest.response = function(headers)
  if HttpRequest.__response then
    for _, header in ipairs(HttpRequest.__response) do
      table.insert(headers, header)
    end
  end
end

return HttpRequest
