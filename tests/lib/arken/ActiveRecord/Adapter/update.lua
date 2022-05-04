local Adapter = require('arken.ActiveRecord.Adapter')
local test = {}

local config  = "config/active_record_sqlite.json"

if not os.exists(config) then
  test.config_not_exists = config
  return test
end

test.should_error_insert = function()
  local status, message = pcall(Adapter.update)
  assert( status == false )
  assert( message:contains('not implemeted') == true )
end

return test
