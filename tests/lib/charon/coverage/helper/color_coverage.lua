local helper = require('charon.coverage.helper')
local test   = {}

test.should_return_ffd4d4_is_value_is_minor_at_100 = function()
  assert(helper.color_coverage(50) == '#ffd4d4')
end

test.should_return_d4dbff_is_value_is_equal_at_100 = function()
  assert(helper.color_coverage(100) == '#d4dbff')
end

return test
