local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  helper.actionName = 'save'
  local result = helper:urlPerform()
  local html   = [[/index/savePerform]]
  assert( result == html, result )
end

test.should_return_html = function()
  helper.controllerName = 'users'
  helper.actionName     = 'save'
  local result = helper:urlPerform()
  local html   = [[/users/savePerform]]
  assert( result == html, result )
end

return test
