local Controller = require('charon.Controller')
local test = {}

test.before = function()
  Controller.prefixHelpers = 'util/helpers/'
end

test.should_return_default_helper_by_metatable = function()
  Controller.prefixHelpers = 'util/disabled/'
  local c = Controller.new{ controller_name = 'controller' }
  local h = c:resolvHelper()
  Controller.prefixHelpers = 'util/helpers/'
  assert( getmetatable(h) == require('charon.Helper') )
end

test.should_return_defaul_helper_if_exists = function()
  local c = Controller.new{ controller_name = 'controller' }
  local h = c:resolvHelper()
  assert( type(h.mydefaulthelper) == 'function', type(h.mydefaulthelper) )
end

test.should_return_function_from_base_helper = function()
  local c = Controller.new{ controller_name = 'controller' }
  local h = c:resolvHelper()
  assert( type(h.nl2br) == 'function', type(h.mydefaulthelper) )
end

test.should_return_helper_from_controller_name = function()
  local MyController = Class.new("MyController", Controller)
  local c = MyController.new{ controller_name = 'my_controller' }
  local h = c:resolvHelper()
  -- base helper
  assert( type(h.nl2br) == 'function', type(h.mydefaulthelper) )
  -- default helper
  assert( type(h.mydefaulthelper) == 'function', type(h.mydefaulthelper) )
  -- named helper
  assert( type(h.mycontrollerhelper) == 'function', type(h.mycontrollerhelper) )
end

return test
