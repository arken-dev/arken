-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Controller   = require 'charon.Controller'
local ActiveRecord = require 'charon.ActiveRecord'
local ByteArray    = require 'charon.ByteArray'
local template     = require 'charon.template'

-------------------------------------------------------------------------------
-- DISPATCHER
-------------------------------------------------------------------------------

request = require('charon.net.request')
local dispatcher = {}

dispatcher.prefix = ""
dispatcher.public = "public"

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
  local controller_name = path:mid(start, last-start)
  local action_name     = path:mid(#controller_name + start + 1)
  local controller_path = path:mid(1, path:len() - action_name:len())
  if controller_name == '' then
    controller_name = 'index'
  end
  if last == 1 or action_name == '' then
    action_name = "index"
  end
  return controller_name, action_name, controller_path
end

-------------------------------------------------------------------------------
-- REQUIRE CONTROLLER
-------------------------------------------------------------------------------

dispatcher.requireController = function(controller_name)
  return require(controller_name:camelCase() .. "Controller")
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
    local status, headers, body = object:pexecute(action_name .. "Action")
    request.response(headers)
    return status, headers, body
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
    local fileName = dispatcher.public .. request.requestPath():mid(#dispatcher.prefix+1, -1)
    if fileName ~= (dispatcher.public .. "/") and os.exists(fileName) then
      return dispatcher.dispatchLocal(fileName)
    else
      reload = package.reload()
      code, headers, body = dispatcher.dispatchController(request)
    end
  else
    code, headers, body = dispatcher.dispatchController(request)
  end
  time = (os.microtime() - time)
  if code == nil then
    error "body empty, missing render or return ?"
  end
  dispatcher.log(code, time, reload)
  dispatcher.after()
  return code, headers, body
end

-------------------------------------------------------------------------------
-- LOG
-------------------------------------------------------------------------------

dispatcher.output = print

dispatcher.log = function(code, time, reload)
  local msg = "Completed in %.4f ms (Reload: %.4f, View: %.4f, DB: %.4f) | %i OK [%s]"
  local log = string.format(msg, time, reload, template.time, ActiveRecord.time, code, request:requestUri())
  dispatcher.output(log)
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
  local status = 200
  local header = {'Content-Type: text/html; charset=utf-8'}
  local body   = "<html><body><h1>DISPATCHER TEST IS WORKS ...</h1></body></html>"
  return status, header, body
end

return dispatcher
