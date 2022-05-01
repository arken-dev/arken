local test   = {}
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local config  = "config/active_record_postgres.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  Person.tableName = string.format("person_%s", os.uuid():replace('-', '_'))
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_postgres.json"
  local sql = [[
  CREATE TABLE %s (
    id SERIAL PRIMARY KEY, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  )]]
  Person.adapter():execute(string.format(sql, Person.tableName))
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  Person.adapter():execute(string.format("DROP TABLE %s", Person.tableName))
  ActiveRecord.config = nil
end

test.define_update_with_different_value_as_created_at = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  p.name = "Rafael dos Anjos"
  p:save()

  assert(p.created_at ~= p.updated_at)
end

return test
