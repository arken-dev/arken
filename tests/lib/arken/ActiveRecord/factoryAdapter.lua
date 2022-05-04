local ActiveRecord = require 'arken.ActiveRecord'
local json = require('arken.json')
local test = {}

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"

  local sql = [[
  CREATE TABLE IF NOT EXISTS user_factory_adapter (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250)
  )]]
  ActiveRecord.adapter():execute(sql)

  local sql = [[
  CREATE TABLE IF NOT EXISTS product_factory_adapter (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, description VARCHAR(250)
  )]]
  ActiveRecord.adapter():execute(sql)

end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.adapter():execute([[DROP TABLE user_factory_adapter]])
  ActiveRecord.adapter():execute([[DROP TABLE product_factory_adapter]])
  ActiveRecord.config = nil
end

test.should_return_adapter_by_table_name = function()
  local user    = ActiveRecord.factoryAdapter{ tableName = 'user_factory_adapter' }
  local product = ActiveRecord.factoryAdapter{ tableName = 'product_factory_adapter' }

  assert( type(user:columns().name) == 'table' )
  assert( user:columns().name.format == 'string' )

  assert( type(product:columns().description) == 'table' )
  assert( product:columns().description.format == 'string' )
end

return test
