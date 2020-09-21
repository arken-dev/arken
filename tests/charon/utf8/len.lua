local utf8 = require 'arken.utf8'
local test = {}

test.should_return_lower_case_string = function()
  local str = 'ALÇA'
  assert( string.len(str) == 5 )
  assert( utf8.len(str) == 4 )
end

return test
