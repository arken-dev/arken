local Array = require('arken.Array')

local test = {}

test.should_return_value = function()
  local ary = Array.new()
  ary.a = 'a'
  ary.b = 'b'
  ary.c = 'c'
  local count = ary:count()
  assert( count == 3, string.format("count %i", count) )
end

return test
