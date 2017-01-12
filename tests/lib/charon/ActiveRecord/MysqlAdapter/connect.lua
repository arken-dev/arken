local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local ActiveRecord = require('charon.ActiveRecord')
local Adapter = require('charon.ActiveRecord.MysqlAdapter')

test.beforeAll = function()
  ActiveRecord.config = "util/config/active_record_mysql_database_invalid.json"
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_error_connect_with_database_invalid = function()
  Adapter.instanceConnection = nil
  local PersonConnect = Class.new("PersonConnect", "ActiveRecord")
  local status, message = pcall( PersonConnect.columns )
  assert( status == false )
  assert( message:contains('error connecting to database. MySQL: Access denied for user') == true, message )
end

return test
