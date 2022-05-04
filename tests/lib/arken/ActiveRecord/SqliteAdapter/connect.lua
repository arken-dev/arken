local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local ActiveRecord = require('arken.ActiveRecord')
local Adapter = require('arken.ActiveRecord.SqliteAdapter')

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "util/config/active_record_sqlite_database_invalid.json"
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_error_connect_with_database_invalid = function()
  Adapter.instanceConnection = nil
  local PersonConnect = Class.new("PersonConnect", "ActiveRecord")
  local status, message = pcall( PersonConnect.columns )
  assert( status == false )
  assert( message:contains('connect to sqlite error: LuaSQL: unable to open database file') == true, message )
end

return test
