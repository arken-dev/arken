local url        = require 'charon.net.url'
local cookie     = require 'charon.net.cookie'
local multipart  = require 'charon.net.multi-part'
local json       = require 'charon.json'
--local HttpParser = require 'charon.net.HttpParser'

request = request or {}
request.converter = require('charon.Converter')

request.params = function(rebuild)
  if request.__params == nil or rebuild then
    if request.requestMethod() == 'POST' then
      if request.field('Content-Type'):startsWith('multipart/form-data;') then
        request.__params = multipart.parse(request.headerDone())
      else
        request.__params = url.parseQuery(request.headerDone())
      end
    else
      request.__params = url.parseQuery(request.queryString())
    end
  end
  return request.converter.new(request.__params)
end

request.cookies = function()
  if request.__cookies == nil then
    request.__cookies = cookie.parse(request.field("Cookie"))
  end
  return request.__cookies
end

request.cookie = function(name, value, params)

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

  request.append(cookie)
end

request.session = function()
  local cookies = request.cookies()
  if cookies.charon_session_id then
    if request.__session_data == nil then
      request.__session_id = cookies.charon_session_id
      if cache.value(request.__session_id) then
        local data = cache.value(request.__session_id)
        if #data == 0 then
          request.__session_data = {}
        else
          request.__session_data = json.decode(data)
        end
      end
    end
  else
    local uuid = os.uuid()
    request.cookie('charon_session_id', uuid)
    request.__session_id   = uuid
    request.__session_data = {}
  end

  return request.__session_data
end

request.reset = function()
  if request.__session_id and type(request.__session_data) == 'table' then
    cache.insert(request.__session_id, json.encode(request.__session_data))
  end
  request.__params       = nil
  request.__response     = nil
  request.__cookies      = nil
  request.__session_id   = nil
  request.__session_data = nil
end

request.append = function(header)
  if request.__response == nil then
    request.__response = {}
  end
  table.insert(request.__response, header)
end

request.response = function(headers)
  if request.__response then
    for _, header in ipairs(request.__response) do
      table.insert(headers, header)
    end
  end
end

return request
