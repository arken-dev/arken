local test    = {}
local json    = require('arken.jsonp')
local Class   = require('arken.oop.Class')
local Adapter = require('arken.ActiveRecord.SqliteAdapter')
local SqliteTypes = Class.new("SqliteTypes", "ActiveRecord")
SqliteTypes.tableName = 'adapter_types'

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  ActiveRecord.schema = "util/schema"
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
  ActiveRecord.schema = false
end

test.shoul_return_columns_with_out_table_created = function()
  local columns = SqliteTypes.columns()
  assert( columns.id.primaryKey == true )
  local default = columns.timestamp_with_default.default
  assert( default == '2015-03-11 13:35:10', default  )
end

test.shoul_return_sql_schema = function()
  local schema = SqliteTypes:adapter():schema()
  local sql    = os.read('util/schema/adapter_types.sql'):trimmed()
  assert( schema == sql, schema )
end

return test
