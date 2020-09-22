local Controller = require('arken.Controller')
local test = {}

test.before = function()
  Controller.prefixViews= 'util/views'
end

test.should_return_value_without_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'html', value = "<h1>Hello...</h1>", layout = false }
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: text/html', headers[1] )
  assert( body == "<h1>Hello...</h1>", body )
end

test.should_return_controllerName_and_default_name = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'html' }
  assert( body == "<h3>Order</h3>", body )
end

test.should_return_controllerName_and_custom_view = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'html', view = 'save' }
  assert( body == "<h3>save...</h3>", body )
end

test.should_return_controllerName_and_custom_path = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  local status, headers, body = c:render{ output = 'html', template = 'mycustom/path' }
  assert( body == "<h3>path...</h3>", body )
end

test.should_return_controllerName_and_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  c.layout = 'test'
  local status, headers, body = c:render({ output = 'html' })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: text/html', headers[1] )
  assert( body == "<html><body>teste<br><h3>Order</h3></body></html>", body:escapeHtml() )
end

test.should_return_value_and_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  c.layout = 'test'
  local status, headers, body = c:render({ output = 'html', value = '<h3>Value</h3>' })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: text/html', headers[1] )
  assert( body == "<html><body>teste<br><h3>Value</h3></body></html>", body:escapeHtml() )
end

test.should_return_error_in_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  c.layout = 'error'
  local status, headers, body = c:render({ output = 'html' })
  assert( status == 500, status )
  assert( headers[1] == 'Content-Type: text/plain', headers[1] )
  assert( body:contains("error_html_layout") == true, body:escapeHtml() )
end

test.should_return_view_and_layout = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  c.layout = 'test'
  local status, headers, body = c:render({ output = 'html' })
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: text/html', headers[1] )
  assert( body == '<html><body>teste<br><h3>Order</h3></body></html>', body:escapeHtml() )
end

test.should_return_error_in_view = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  c.layout = 'test'
  local status, headers, body = c:render{ output = 'html', view = 'error' }
  assert( status == 500, status )
  assert( headers[1] == 'Content-Type: text/plain', headers[1] )
  assert( body:contains("error_html_view") == true, body:escapeHtml() )
end

return test
