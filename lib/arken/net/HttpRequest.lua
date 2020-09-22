local url         = require 'arken.net.url'
local cookie      = require 'arken.net.cookie'
local multipart   = require 'arken.net.multi-part'
local json        = require 'arken.json'
local Class       = require 'arken.oop.Class'

local HttpRequest = Class.new("arken.net.HttpRequest")

HttpRequest.converter = require('arken.Converter')

function HttpRequest:env()
  return self._env
end

function HttpRequest:params(rebuild)
  if self._params == nil or rebuild then
    if self._env:requestMethod() == 'POST' then
      if self._env:field('Content-Type'):startsWith('multipart/form-data;') then
        self._params = multipart.parse(self._env:headerDone())
      else
        self._params = url.parseQuery(self._env:headerDone())
      end
    else
      self._params = url.parseQuery(self._env:queryString())
    end
    self._params = self.converter.new(self._params)
  end
  return self._params
end

function HttpRequest:cookies(rebuild)
  if self._cookies == nil or rebuild then
    self._cookies = cookie.parse(self._env:field("Cookie"))
  end
  return self._cookies
end

function HttpRequest:cookie(name, value, params)

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

  self:append(cookie)
end

function HttpRequest:session()
  local cookies = self:cookies()
  if cookies.arken_session_id then
    if self._session_data == nil then
      self._session_id = cookies.arken_session_id
      if cache.value(self.__session_id) then
        local data = cache.value(self._session_id)
        if #data == 0 then
          self._session_data = {}
        else
          self._session_data = json.decode(data)
        end
      end
    end
  else
    local uuid = os.uuid()
    self:cookie('arken_session_id', uuid)
    self._session_id   = uuid
    self._session_data = {}
  end

  return self._session_data
end

function HttpRequest:append(header)
  if self._response == nil then
    self._response = {}
  end
  table.insert(self._response, header)
end

function HttpRequest:response(headers)
  if self._session_id and type(self._session_data) == 'table' then
    cache.insert(self._session_id, json.encode(self._session_data))
  end

  if self._response then
    for _, header in ipairs(self._response) do
      table.insert(headers, header)
    end
  end
end

return HttpRequest
