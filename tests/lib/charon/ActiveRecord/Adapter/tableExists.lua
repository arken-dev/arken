local Adapter = require('charon.ActiveRecord.Adapter')
local test = {}

test.should_error_insert = function()
  local status, message = pcall(Adapter.tableExists)
  assert( status == false )
  assert( message:contains("tableExists not implemented") == true )
end

return test
