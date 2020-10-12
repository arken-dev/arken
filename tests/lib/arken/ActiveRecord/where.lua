local ActiveRecord = require('arken.ActiveRecord')
local test = {}

test.beforeAll = function()
  ActiveRecord.reset()
end

test.should_return_error_default = function()
  local status, message = pcall(ActiveRecord.where)
  assert( status == false )
  assert( message:contains('where not implemented in ActiveRecord') == true, message )
end

return test
