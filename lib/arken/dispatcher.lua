-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Controller   = require 'arken.Controller'
local ActiveRecord = require 'arken.ActiveRecord'
local template     = require 'arken.template'
local HttpEnv      = require 'arken.net.HttpEnv'

-------------------------------------------------------------------------------
-- DISPATCHER
-------------------------------------------------------------------------------

local dispatcher = {}
dispatcher.prefix = ""
dispatcher.public = "public"

-------------------------------------------------------------------------------
-- PARSE PATH
-------------------------------------------------------------------------------

dispatcher.parsePath  = function(env)
  local path  = env:requestPath()
  local last  = path:lastIndexOf('/')
  local start = 2
  if dispatcher.prefix then
    start = start + #dispatcher.prefix
  end
  local controllerName = path:mid(start, last-start)
  local actionName     = path:mid(#controllerName + start + 1)
  local controllerPath = path:mid(1, path:len() - actionName:len())
  if controllerName == '' then
    controllerName = 'index'
  end
  if last == 1 or actionName == '' then
    actionName = "index"
  end
  return controllerName, actionName:camelCase(true), controllerPath
end

-------------------------------------------------------------------------------
-- REQUIRE CONTROLLER
-------------------------------------------------------------------------------

dispatcher.requireController = function(controllerName)
  return require(controllerName:camelCase() .. "Controller")
end

-------------------------------------------------------------------------------
-- DISPATCHER LOCAL
-- dispatcher local files in dir public in development mode
-------------------------------------------------------------------------------

dispatcher.dispatchLocal = function(fileName)
  local list     = require 'arken.net.mime-type'
  local suffix   = fileName:suffix('.')
  local mimetype = tostring(list[suffix])
  local header   = "Content-type: " .. mimetype
  local file     = io.open(fileName, "rb")
  return 200, {header}, file:read("*all")
end

-------------------------------------------------------------------------------
-- DISPATCHER CONTROLLER
-------------------------------------------------------------------------------

dispatcher.dispatchController = function(env)
  local controllerName, actionName, controllerPath = dispatcher.parsePath(env)
  local class  = dispatcher.requireController(controllerName)
  local object = class.new{
    controllerName = controllerName,
    actionName     = actionName,
    controllerPath = controllerPath,
    _env = env
  }
  if object[actionName .. "Action"] then
    return object:pexecute(actionName .. "Action")
  else
    return 500, {}, "action: \"" .. actionName .. "Action\" not found"
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

dispatcher.dispatch = function(env)
  dispatcher.before()
  local time    = os.microtime()
  local reload  = 0
  local code, headers, body
  if ARKEN_ENV == 'development' then
    local requestPath = env:requestPath()
    local fileName    = dispatcher.public .. requestPath:mid(#dispatcher.prefix+1, -1)
    if requestPath ~= '/' and os.exists(fileName) then
      return dispatcher.dispatchLocal(fileName)
    else
      reload = package.reload()
      code, headers, body = dispatcher.dispatchController(env)
    end
  else
    code, headers, body = dispatcher.dispatchController(env)
  end
  time = (os.microtime() - time)
  if code == nil then
    error "body empty, missing render or return ?"
  end
  dispatcher.log(env, code, time, reload)
  dispatcher.after()
  return code, headers, body
end

-------------------------------------------------------------------------------
-- LOG
-------------------------------------------------------------------------------

dispatcher.output = print

dispatcher.log = function(env, code, time, reload)
  if ARKEN_ENV ~= 'test' then
    local msg = "Completed in %.4f ms (Reload: %.4f, View: %.4f, DB: %.4f) | %i OK [%s]"
    local log = string.format(msg, time, reload, template.time, ActiveRecord.time, code, env:requestUri())
    dispatcher.output(log)
  end
end

-------------------------------------------------------------------------------
-- AFTER
-------------------------------------------------------------------------------

dispatcher.after = function(env)
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
