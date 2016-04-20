require 'Controller'
require 'OByteArray'

local url = require 'url'

request.params = function()
  if request.requestMethod() == 'POST' then
   return url.parseQuery(request.headerDone())
  else
  return url.parseQuery(request.queryString())
  end
end

local M = {}

M.parse_path = function()
  local path = request:requestPath()
  local last = path:lastIndexOf('/')
  local controller = path:mid(1, last-1)
  local action     = path:right(path:len() - last - 1)
  if controller == '' then
    controller = 'index'
  end
  if last == 0 or action == '' then
    action = "index"
  end
  return controller, action
end

M.require_controller_name = function(controller_name)
  return require(controller_name .. "_controller")
end

M.dispatchLocal = function(file_name)
  print("dispatcher: " .. file_name)
  local list     = require 'oberon.mime-type'
  local fileInfo = QFileInfo.new(file_name)
  local suffix   = fileInfo:suffix()
  local mimetype = tostring(list[suffix])
  local header   = "Content-type: " .. mimetype
  local file     = io.open(file_name, "rb")
  return 200, {header}, file:read("*all")
end

M.dispatchController = function()
  controller_name, action_name = M.parse_path()
  local class  = M.require_controller_name(controller_name)
  local object = class.new{controller_name = controller_name, action_name = action_name}
  if object[action_name .. "Action"] then
    return object:execute(action_name .. "Action")
  else
    return 200, {}, "action: \"" .. action_name .. "Action\" not found"
  end
end

M.dispatch = function()
  local file_name = "public" .. request.requestPath()
  if file_name ~= "public/" and QFile.exists(file_name) then
    return M.dispatchLocal(file_name)
  else
    return M.dispatchController()
  end
end

M.test = function()
  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    "<html><body><h1>DISPATCHER TEST IS WORKS ...</h1></body></html>"
end

return M
