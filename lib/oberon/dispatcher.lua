require 'Controller'
require 'OByteArray'
require 'OHttpRequest'
require 'ActiveRecord'

local template = require 'template'
local url      = require 'url'
local cookie   = require 'oberon.cookie'

request.params = function()
  if request.requestMethod() == 'POST' then
   return url.parseQuery(request.headerDone())
  else
  return url.parseQuery(request.queryString())
  end
end

request.cookies = function()
  return cookie.parse(request.field("Cookie"))
end

local M = {}

M.prefix = nil

M.parse_path  = function()
  local path  = request:requestPath()
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

M.dispatchLocal = function(file_name)
  local list     = require 'oberon.mime-type'
  local fileInfo = QFileInfo.new(file_name)
  local suffix   = fileInfo:suffix()
  local mimetype = tostring(list[suffix])
  local header   = "Content-type: " .. mimetype
  local file     = io.open(file_name, "rb")
  return 200, {header}, file:read("*all")
end

M.dispatchController = function()
  controller_name, action_name, controller_path = M.parse_path()
  local class  = M.require_controller_name(controller_name)
  local object = class.new{controller_name = controller_name, action_name = action_name, controller_path = controller_path}
  if object[action_name .. "Action"] then
    return object:pexecute(action_name .. "Action")
  else
    return 200, {}, "action: \"" .. action_name .. "Action\" not found"
  end
end

M.dispatch = function()
  ActiveRecord.time = 0
  template.time = 0
  local time    = os.microtime()
  local reload  = 0
  local code, headers, body
  if OBERON_ENV == 'development' then
    local file_name = "public" .. request.requestPath()
    if file_name ~= "public/" and QFile.exists(file_name) then
      return M.dispatchLocal(file_name)
    else
      reload = package.reload()
      template.reload()
      code, headers, body = M.dispatchController()
    end
  else
    code, headers, body = M.dispatchController()
  end
  time = (os.microtime() - time)
  if code == nil then
    error "body empty, render ?"
  end
  M.log(code, time, reload)
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
