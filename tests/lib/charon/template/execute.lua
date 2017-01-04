local template = require 'charon.template'
local test = {}

test.should_return_function = function()
  local result = template.execute("util/template/example-equal-char.tpl", {})
  assert( result == 'Hello World !!!', result )
end

return test
