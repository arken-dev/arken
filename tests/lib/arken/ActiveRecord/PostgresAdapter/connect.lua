local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local ActiveRecord = require('arken.ActiveRecord')
local Adapter = require('arken.ActiveRecord.PostgresAdapter')
local config  = "config/active_record_postgres.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "util/config/active_record_postgres_database_invalid.json"
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_error_connect_with_database_invalid = function()
  Adapter.instanceConnection = nil
  local PostgresConnect = Class.new("PostgresConnect", "ActiveRecord")
  local status, message = pcall( PostgresConnect.columns )
  assert( status == false )
  assert( message:contains('LuaSQL: error connecting to database. PostgreSQL: FATAL:  role "userunknow" does not exist') == true, message )
end

return test
