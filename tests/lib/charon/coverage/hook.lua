local test = {}
local coverage = require('charon.coverage')
local mydebug  = {}
mydebug.info = function()
  return "@lib/charon/coverage.lua", 2
end

test.should_apply_linedefined = function()
  coverage.hook('', '', mydebug)
  coverage.hook('', '', mydebug)
  local result = coverage.dump()
  assert( result['@lib/charon/coverage.lua'][2] == 1, result['@lib/charon/coverage.lua'][2] )
end

return test
