local ActiveRecord = require 'arken.ActiveRecord'
local test = {}

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.should_error_if_file_not_exists = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/file_not_exists.json"
  local status, message = pcall(ActiveRecord.loadConfig)
  assert( status == false )
  assert( message:contains('file config/file_not_exists.json not exists') == true, message )
end

test.should_error_if_file_not_contain_json_table = function()
  ActiveRecord.config = "util/config/active-record-not-valid.json"
  local status, message = pcall(ActiveRecord.loadConfig)
  assert( status == false )
  assert( message:contains('util/config/active-record-not-valid.json invalid') == true, message )
end

test.should_return_test_environment = function()
  ActiveRecord.config = "util/config/active-record-valid.json"
  local config = ActiveRecord.loadConfig()
  assert(config.database == ':memory:', config.database )
end

return test
