local dispatcher = require('charon.dispatcher')
local test = {}
package.path = package.path .. ';util/?.lua'

test.should_controller_and_action_and_path = function()
  local request = {}
  request.requestPath = function()
    return "/order/save"
  end
  dispatcher.prefix = ''
  local controller_name, action_name, controller_path = dispatcher.parsePath(request)
  assert( controller_name == 'order', controller_name )
  assert( action_name == 'save', action_name )
  assert( controller_path == '/order/', controller_path )
end

test.should_controller_and_action_and_path_with_prefix = function()
  local request = {}
  request.requestPath = function()
    return "/app/order/save"
  end
  dispatcher.prefix = '/app'
  local controller_name, action_name, controller_path = dispatcher.parsePath(request)
  assert( controller_name == 'order', controller_name )
  assert( action_name == 'save', action_name )
  assert( controller_path == '/app/order/', controller_path )
end

test.should_controller_and_action_empty_with_prefix = function()
  local request = {}
  request.requestPath = function()
    return "/app"
  end
  dispatcher.prefix = '/app'
  local controller_name, action_name, controller_path = dispatcher.parsePath(request)
  assert( controller_name == 'index', controller_name )
  assert( action_name == 'index', action_name )
  assert( controller_path == '/app', controller_path )
end

test.should_controller_and_action_empty_with_prefix_and_bar = function()
  local request = {}
  request.requestPath = function()
    return "/app/"
  end
  dispatcher.prefix = '/app'
  local controller_name, action_name, controller_path = dispatcher.parsePath(request)
  assert( controller_name == 'index', controller_name )
  assert( action_name == 'index', action_name )
  assert( controller_path == '/app/', controller_path )
end

test.should_controller_and_action_empty_without_prefix = function()
  local request = {}
  request.requestPath = function()
    return ""
  end
  dispatcher.prefix = ''
  local controller_name, action_name, controller_path = dispatcher.parsePath(request)
  assert( controller_name == 'index', controller_name )
  assert( action_name == 'index', action_name )
  assert( controller_path == '', controller_path )
end

return test
