local contract = require('arken.contract')
local test     = {}

test.should_erase_table_errors = function()
  local model  = {}
  model.errors = {}
  model.errors.total = 'is a total error'
  contract.clear(model)
  assert( model.errors.total == nil )
end

return test
