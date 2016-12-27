local helper = require('charon.coverage.helper')
local test   = {}

test.should_return_strig_whitout_bar = function()
  assert( helper.parameterize('/Users/john') == "-Users-john")
end

return test
