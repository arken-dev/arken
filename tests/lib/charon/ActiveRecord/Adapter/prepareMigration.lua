local Adapter = require('charon.ActiveRecord.Adapter')
local test = {}

test.should_error_insert = function()
  local status, message = pcall(ActiveRecord_Adapter.prepareMigration)
  assert( status == false )
  assert( message:contains("prepare migrations not implemented") == true )
end

return test
