local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local ActiveRecord = require('charon.ActiveRecord')
local Adapter = require('charon.ActiveRecord.PostgresAdapter')

test.beforeAll = function()
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
