local coverage = require('arken.coverage')
local test = {}

test.should_return_a_table = function()
  assert( type(coverage.dump()) == 'table' )
end

return test
