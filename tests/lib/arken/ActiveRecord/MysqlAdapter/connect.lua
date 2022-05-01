local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local ActiveRecord = require('arken.ActiveRecord')
local Adapter = require('arken.ActiveRecord.MysqlAdapter')
local config  = "config/active_record_mysql.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
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
