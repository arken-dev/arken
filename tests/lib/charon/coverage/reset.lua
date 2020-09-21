local coverage = require('arken.coverage')
local test = {}

test.should_return_a_nil = function()
  assert( coverage.reset() == nil )
end

test.should_return_differente_table_is_a_dump = function()
  local result = coverage.dump()
  coverage.reset()
  assert( result ~= coverage.dump() )
end

return test
