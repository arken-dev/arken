local Array = require('charon.Array')

local test = {}

test.should_return_value = function()
  local ary = Array.new{ 'a', 'b', 'c' }
  assert( ary:size() == 3 )
end

return test
