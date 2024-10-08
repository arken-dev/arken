local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local ActiveRecord = require('arken.ActiveRecord')

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_create_table_schema_migration = function()
  assert( ActiveRecord.adapter():tableExists('schema_migration') == false )
  ActiveRecord.adapter():prepareMigration()
  assert( ActiveRecord.adapter():tableExists('schema_migration') == true )
end

test.should_return_empty_list = function()
  assert( ActiveRecord.adapter():tableExists('schema_migration') == false )
  local list = ActiveRecord.adapter():prepareMigration()
  assert( #list == 0 )
  assert( type(list) == 'table' )
end

test.should_return_list = function()
  ActiveRecord.adapter():prepareMigration()
  local version = '2017-01-01'
  ActiveRecord.adapter():execute(string.format([[INSERT INTO schema_migration VALUES ('%s')]], version))
  local list = ActiveRecord.adapter():prepareMigration()
  assert( type(list) == 'table' )
  assert( list[version] == true )
end

return test
