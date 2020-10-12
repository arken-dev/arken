local test    = {}
local json    = require('arken.jsonp')
local Class   = require('arken.oop.Class')
local Adapter = require('arken.ActiveRecord.SqliteMemoryAdapter')
local SqliteTypes = Class.new("SqliteTypes", "ActiveRecord")
SqliteTypes.tableName = 'adapter_types'

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config   = "config/active_record_sqlite.json"
  ActiveRecord.schema   = "util/schema"
  ActiveRecord.fixtures = "util/fixtures"
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

test.shoul_loader_fixtures = function()
  Adapter.loader()
  local cursor = ActiveRecord.adapter():execute("SELECT * FROM adapter_types")
  local result = cursor:fetch({}, 'a')
  assert( result.name == 'John', result.name )
end

return test
