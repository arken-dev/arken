local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local ActiveRecord = require('charon.ActiveRecord')
local Adapter = require('charon.ActiveRecord.SqliteAdapter')

test.beforeAll = function()
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
