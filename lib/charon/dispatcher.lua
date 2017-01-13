-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Controller   = require 'charon.Controller'
local ActiveRecord = require 'charon.ActiveRecord'
local ByteArray    = require 'charon.ByteArray'
local HttpParser   = require 'charon.net.HttpParser'
local multipart    = require 'charon.net.multi-part'
local url          = require 'charon.net.url'
local cookie       = require 'charon.net.cookie'
local template     = require 'charon.template'

-------------------------------------------------------------------------------
-- DISPATCHER
-------------------------------------------------------------------------------

local dispatcher = {}
dispatcher.converter = require('charon.Converter')
dispatcher.prefix = ""

-------------------------------------------------------------------------------
-- REQUEST
-------------------------------------------------------------------------------
request = request or {}

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
  return dispatcher.converter.new(request.__params)
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

-------------------------------------------------------------------------------
-- PARSE PATH
-------------------------------------------------------------------------------

dispatcher.parsePath  = function(request)
  local path  = request.requestPath()
  local last  = path:lastIndexOf('/')
  local start = 2
  if dispatcher.prefix then
    start = start + #dispatcher.prefix
  end
  local controller = path:mid(start, last-start)
  local action     = path:mid(#controller + start + 1)
  local path       = path:mid(1, path:len() - action:len())
  if controller == '' then
    controller = 'index'
  end
  if last == 1 or action == '' then
    action = "index"
  end
  return controller, action, path
end

-------------------------------------------------------------------------------
-- REQUIRE CONTROLLER
-------------------------------------------------------------------------------

dispatcher.requireController = function(controller_name)
  return require(controller_name:camelcase() .. "Controller")
end

-------------------------------------------------------------------------------
-- DISPATCHER LOCAL
-- dispatcher local files in dir public in development mode
-------------------------------------------------------------------------------

dispatcher.dispatchLocal = function(fileName)
  local list     = require 'charon.net.mime-type'
  local suffix   = fileName:suffix()
  local mimetype = tostring(list[suffix])
  local header   = "Content-type: " .. mimetype
  local file     = io.open(fileName, "rb")
  return 200, {header}, file:read("*all")
end

-------------------------------------------------------------------------------
-- DISPATCHER CONTROLLER
-------------------------------------------------------------------------------

dispatcher.dispatchController = function(request)
  local controller_name, action_name, controller_path = dispatcher.parsePath(request)
  local class  = dispatcher.requireController(controller_name)
  local object = class.new{
    controller_name = controller_name,
    action_name     = action_name,
    controller_path = controller_path,
    request         = request
  }
  if object[action_name .. "Action"] then
    return object:pexecute(action_name .. "Action")
  else
    return 500, {}, "action: \"" .. action_name .. "Action\" not found"
  end
end

-------------------------------------------------------------------------------
-- BEFORE CALLBACK
-------------------------------------------------------------------------------

dispatcher.before = function()
  ActiveRecord.time = 0
  template.time = 0
end

-------------------------------------------------------------------------------
-- DISPATCH
-------------------------------------------------------------------------------

dispatcher.dispatch = function()
  dispatcher.before()
  local time    = os.microtime()
  local reload  = 0
  local code, headers, body
  if CHARON_ENV == 'development' then
    local fileName = "public" .. request.requestPath():mid(#dispatcher.prefix+1, -1)
    if fileName ~= "public/" and os.exists(fileName) then
      return dispatcher.dispatchLocal(fileName)
    else
      reload = package.reload()
      code, headers, body = dispatcher.dispatchController(request)
    end
  else
    code, headers, body = dispatcher.dispatchController(request)
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
  dispatcher.log(code, time, reload)
  dispatcher.after()
  return code, headers, body
end

-------------------------------------------------------------------------------
-- LOG
-------------------------------------------------------------------------------

dispatcher.log = function(code, time, reload)
  local msg = "Completed in %.4f ms (Reload: %.4f, View: %.4f, DB: %.4f) | %i OK [%s]"
  local log = string.format(msg, time, reload, template.time, ActiveRecord.time, code, request:requestUri())
  print(log)
end

-------------------------------------------------------------------------------
-- AFTER
-------------------------------------------------------------------------------

dispatcher.after = function()
  request.reset()
  ActiveRecord.clear()
  ActiveRecord.time = 0
  template.time     = 0
end

-------------------------------------------------------------------------------
-- TEST
-------------------------------------------------------------------------------

dispatcher.test = function()
  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    "<html><body><h1>DISPATCHER TEST IS WORKS ...</h1></body></html>"
end

return dispatcher
