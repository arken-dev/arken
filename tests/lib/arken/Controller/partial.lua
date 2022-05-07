local Controller = require('arken.Controller')
local test = {}

test.before = function()
  Controller.prefixViews= 'util/views'
end

test.should_render_view_preprend_underline = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  -- util/view/order/_mypartial
  local result = c:partial{ view = 'mypartial' }
  assert( result == '<h3>my partial</h3>\n', result )
end

test.should_render_custom_path_preprend_underline = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  -- util/view/order/_mypartial
  local result = c:partial{ view = 'mypartial' }
  assert( result == '<h3>my partial</h3>\n', result )
end

test.should_render_actionName_preprend_underline = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  -- util/view/order/_index
  local result = c:partial()
  assert( result == '<h3>index partial</h3>\n', result )
end

test.should_render_error = function()
  local c = Controller.new{ controllerName = 'order', actionName = 'index' }
  -- util/view/order/_mypartial
  local status, result = pcall(c.partial, c, { view = 'error' })
  assert( status == false )
  assert( result:contains('my_partial_error') == true, result )
end

return test
