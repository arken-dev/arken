-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

require 'Controller'
require 'charon.ActiveRecord'

require 'CByteArray'
require 'CHttpParser'

local url      = require 'charon.net.url'
local cookie   = require 'charon.net.cookie'
local template = require 'template'

request.params = function()
  if request.__params == nil then
    if request.requestMethod() == 'POST' then
      request.__params = url.parseQuery(request.headerDone())
    else
      request.__params = url.parseQuery(request.queryString())
    end
  end
  return request.__params
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
      request.__session_id   = cookies.charon_session_id
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

local M = {}

M.prefix = nil

M.parse_path  = function()
  local path  = request.requestPath()
  local last  = path:lastIndexOf('/')
  local start = 1
  if M.prefix then
    start = start + #M.prefix + 1
  end
  local controller = path:mid(start, last-start)
  local action     = path:right(path:len() - last - 1)
  local path       = path:mid(0, path:len() - action:len())
  if controller == '' then
    controller = 'index'
  end
  if last == 0 or action == '' then
    action = "index"
  end
  return controller, action, path
end

M.require_controller_name = function(controller_name)
  return require(controller_name:camelcase() .. "Controller")
end

M.dispatchLocal = function(fileName)
  local list     = require 'charon.mime-type'
  local suffix   = fileName:suffix()
  local mimetype = tostring(list[suffix])
  local header   = "Content-type: " .. mimetype
  local file     = io.open(fileName, "rb")
  return 200, {header}, file:read("*all")
end

M.dispatchController = function()
  local controller_name, action_name, controller_path = M.parse_path()
  local class  = M.require_controller_name(controller_name)
  local object = class.new{controller_name = controller_name, action_name = action_name, controller_path = controller_path}
  if object[action_name .. "Action"] then
    return object:pexecute(action_name .. "Action")
  else
    return 500, {}, "action: \"" .. action_name .. "Action\" not found"
  end
end

M.dispatch = function()
  ActiveRecord.time = 0
  template.time = 0
  local time    = os.microtime()
  local reload  = 0
  local code, headers, body
  if CHARON_ENV == 'development' then
    local fileName = "public" .. request.requestPath()
    if fileName ~= "public/" and os.exists(fileName) then
      return M.dispatchLocal(fileName)
    else
      reload = package.reload()
      template.reload()
      code, headers, body = M.dispatchController()
    end
  else
    code, headers, body = M.dispatchController()
  end
  if request.__response then
    for _, header in ipairs(request.__response) do
      table.insert(headers, header)
    end
  end
  time = (os.microtime() - time)
  if code == nil then
    error "body empty, render ?"
  end
  M.log(code, time, reload)
  request.reset()
  return code, headers, body
end

M.log = function(code, time, reload)
  local msg = "Completed in %.4f ms (Reload: %.4f, View: %.4f, DB: %.4f) | %i OK [%s]"
  local log = string.format(msg, time, reload, template.time, ActiveRecord.time, code, request:requestUri())
  M.reset()
  print(log)
end

M.reset = function()
  ActiveRecord.clear()
  ActiveRecord.time = 0
  template.time     = 0

  return true
end

M.test = function()
  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    "<html><body><h1>DISPATCHER TEST IS WORKS ...</h1></body></html>"
end

return M
