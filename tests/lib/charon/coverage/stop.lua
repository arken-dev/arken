local coverage = require('arken.coverage')
local test     = {}
local mydebug  = {}

mydebug.sethook = function(val1, val2)
  mydebug.val1 = val1
  mydebug.val2 = val2
end

test.should_apply_stop_hooks = function()
  coverage.stop(mydebug)
  assert(mydebug.val1 == nil)
  assert(mydebug.val2 == 'l')
end

test.should_return_result_files_is_a_absolute_path = function()
  local result = coverage.dump()
  result["@lib/coverage.lua"] = "this is a content"
  coverage.stop(mydebug)
  result = coverage.dump()
  local abspath = os.abspath("lib/coverage.lua")
  assert( result[abspath] == "this is a content" )
end

return test
