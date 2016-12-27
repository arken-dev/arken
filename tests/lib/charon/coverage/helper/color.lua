local helper = require('charon.coverage.helper')
local test   = {}

test.should_return_ffd4d4_if_value_is_nil = function()
  assert( helper.color(nil) == '#ffd4d4' )
end

test.should_return_d4dbff_if_value_is_one = function()
  assert( helper.color(1) == '#d4dbff' )
end

test.should_return_ffffff_if_value_is_one_negative = function()
  assert( helper.color(-1) == '#ffffff' )
end

return test
