local Controller = require('charon.Controller')
local test = {}

test.should_return_url_default_action_index = function()
  local c = Controller.new{ controllerName = 'index' }
  assert( c:url({}) == '/index/index', c:url({}) )
end

test.should_return_url_actionNamed = function()
  local c = Controller.new{ controllerName = 'index' }
  assert( c:url({action = 'save'}) == '/index/save', c:url{ action = 'save'} )
end

test.should_return_url_action_with_query_string = function()
  local c = Controller.new{ controllerName = 'index' }
  local result = c:url({action = 'save', id = 1, name = 'John' })
  assert(  result == '/index/save?id=1&name=John', result )
end

return test
