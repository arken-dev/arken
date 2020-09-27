local template = require 'arken.template'
local test = {}

test.should_return_function = function()
  local result = template.build("util/template/example1.tpl", {})
  assert( type(result) == 'function' )
end

return test
