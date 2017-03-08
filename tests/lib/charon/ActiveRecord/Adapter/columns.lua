local Adapter = require('charon.ActiveRecord.Adapter')
local test = {}

test.beforeAll = function()
  ActiveRecord.reset()
end

test.should_error_insert = function()
  local status, message = pcall(Adapter.columns)
  assert( status == false )
  assert( message:contains('not implemeted') == true )
end

return test
