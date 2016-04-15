require 'Controller'
require 'OByteArray'

local M = {}

M.parse_path = function()
  local path = OByteArray.new(request['Path'])
  local last = path:lastIndexOf('/')
  local controller = path:mid(1, last-1)
  local action     = path:right(path:size() - last - 1)
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

M.dispatch = function()
  controller_name, action_name = M.parse_path()
  local class  = M.require_controller_name(controller_name)
  local object = class.new{controller_name = controller_name, action_name = action_name}
  if object[action_name .. "Action"] then
    return object:execute(action_name .. "Action")
  else
    return 200, {}, "action: \"" .. action_name .. "\" not found"
  end
end

M.test = function()
  return 200,
    {'Content-Type: text/html; charset=utf-8'},
    "<html><body><h1>DISPATCHER TEST IS WORKS ...</h1></body></html>"
end

return M
