local test = {}
local coverage = require('charon.coverage')
local mydebug  = {}
mydebug.info = function()
  return "@lib/charon/dontexistsfile.lua", 2
end

test.should_apply_linedefined = function()
  coverage.reset()
  coverage.hook('', '', mydebug)
  coverage.hook('', '', mydebug)
  local result = coverage.dump()
  assert( result['@lib/charon/dontexistsfile.lua'][2] == 2, result['@lib/charon/dontexistsfile.lua'][2] )
end

return test
