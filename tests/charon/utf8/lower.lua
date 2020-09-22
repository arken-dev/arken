local utf8 = require 'arken.utf8'
local test = {}

test.should_return_lower_case_string = function()
  local str = 'ALÇA'
  assert( utf8.lower(str) == 'alça' )
end

return test
