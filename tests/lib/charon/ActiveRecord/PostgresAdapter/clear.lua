local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.table_name = string.format("person_%s", os.uuid():replaceChars('-', '_'))

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_postgres.json"
  local sql = [[
  CREATE TABLE %s (
    id SERIAL PRIMARY KEY, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  )]]
  Person.adapter():execute(string.format(sql, Person.table_name))
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  Person.adapter():execute(string.format("DROP TABLE %s", Person.table_name))
  ActiveRecord.config = nil
end

test.should_reset_instances_storages = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local record1 = Person.find{ id = p.id }
  assert( p == record1 , json.encode(record1) )
  ActiveRecord.clear()
  local record2 = Person.find{ id = p.id }
  assert( p ~= record2 , json.encode(record2) )
end

return test
