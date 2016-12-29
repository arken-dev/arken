local utf8 = require 'charon.utf8'
local test = {}

test.should_return_lower_case_string = function()
  local str = 'alça'
  assert( utf8.upper(str) == 'ALÇA' )
end

return test
