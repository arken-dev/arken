local Controller = require('arken.Controller')
local test = {}

test.before = function()
  Controller.prefixViews= 'util/views'
end

test.should_return_controllerName_and_default_name = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'js' }
  assert( body == [[var controller_name = "order"; var index_name = "index";]], body )
end

test.should_return_controllerName_and_name_view = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'js', view = 'save' }
  assert( body == [[var save = true;]], body )
end

test.should_return_template = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'js', template = 'mycustom/path' }
  assert( body == [[var customPath = true;]], body )
end

test.should_return_status_200 = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'js' }
  assert( status == 200 )
end

test.should_return_status_content_type = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'js' }
  assert( headers[1] == 'Content-Type: text/javascript', headers[1] )
end

test.should_return_js_view_with_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  c.layout = 'test'
  local status, headers, body = c:render{ output = 'js' }
  assert( body == [[var layout = true; var controller_name = "order"; var index_name = "index";]], body )
end

test.should_return_debug_template_if_is_a_error = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'error' }
  local status, headers, body = c:render{ output = 'js' }
  assert( status == 500, status )
  assert( headers[1] == 'Content-Type: text/plain', headers[1])
  assert( body:contains([[attempt to call global 'error_in_line_one']]) == true, body)
end

test.should_return_debug_template_if_is_a_error_in_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'error' }
  c.layout = 'error_layout'
  local status, headers, body = c:render{ output = 'js' }
  assert( status == 500, status )
  assert( headers[1] == 'Content-Type: text/plain', headers[1])
  assert( body:contains([[attempt to call global 'error_in_line_one']]) == true, body)
end

return test
