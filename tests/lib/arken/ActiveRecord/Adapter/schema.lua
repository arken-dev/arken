local Adapter = require('arken.ActiveRecord.Adapter')
local test = {}

test.should_error_not_implemented = function()
  local status, message = pcall(Adapter.schema)
  assert( status == false )
  assert( message:contains("not implemented") == true, message )
end

return test
