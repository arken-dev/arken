local toboolean = require('arken.toboolean')
local test = {}

test.should_return_true_for_values_representating = function()
  assert( toboolean('true') == true )
  assert( toboolean('t')    == true )
  assert( toboolean(true)   == true )
  assert( toboolean(1)      == true )
end

test.should_return_false_for_values_representating = function()
  assert( toboolean('false') == false )
  assert( toboolean('f')     == false )
  assert( toboolean(false)   == false )
  assert( toboolean(0)       == false )
end

test.should_return_nil_for_values_not_unknown = function()
  assert( toboolean('unknown') == nil )
  assert( toboolean(-1) == nil )
  assert( toboolean(2) == nil )
end

return test
