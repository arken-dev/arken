local test = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.tableName = string.format("person_%s", 'test_all')

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE %s (
    id INTEGER PRIMARY KEY AUTO_INCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  ) ENGINE=InnoDB]]
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

test.should_return_record_stored = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local result = Person.all{ name = "Chris Weidman" }
  assert( result[1].name == 'Chris Weidman', json.encode(result) )
end

test.should_return_instance_by_primaryKey = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local result = Person.all{ id = p.id }

  assert( p == result[1], tostring(p) .. ' ' .. tostring(result) )
end

test.should_return_one_id = function()
  local p = Person.new()
  p.name = "Ed Alvares"
  p:save()
  local result = Person.all{ name = "Ed Alvares" }
  assert( p.id == result[1].id, string.format('%i %i', p.id, result[1].id) )
end

test.should_return_instance_by_other_attribute = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local result = Person.all{ name = "Junior Cigano" }

  assert( p == result[1], tostring(p) .. ' ' .. tostring(result[1]) )
end

return test
