local Adapter = require('arken.ActiveRecord.Adapter')
local test = {}

local config  = "config/active_record_sqlite.json"

if not os.exists(config) then
  test.config_not_exists = config
  return test
end

test.should_error_not_implemented = function()
  local status, message = pcall(Adapter.schema)
  assert( status == false )
  assert( message:contains("not implemented") == true, message )
end

return test
