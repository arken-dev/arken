local coverage = require('charon.coverage')
local test = {}

test.should_apply_hook_function = function()
  local mydebug = {}
  mydebug.sethook = function(val1, val2)
    mydebug.val1 = val1
    mydebug.val2 = val2
  end
  coverage.start(mydebug)
  assert( mydebug.val1 == coverage.hook )
end

return test
