local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.table_name = string.format("person_%s", os.uuid():replace('-', '_'))

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE %s (
    id INTEGER PRIMARY KEY AUTO_INCREMENT, name VARCHAR(250), observation TEXT,
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

test.should_return_record_stored = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local record = Person.find{ name = "Chris Weidman" }
  assert( record.name == 'Chris Weidman', json.encode(record) )
end

test.should_return_instance_by_primary_key = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local record = Person.find{ id = p.id }

  assert( p == record, string.format('%s %s', p.id, record.id) )
end

test.should_return_one_id = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local record = Person.find{ name = "Junior Cigano" }

  assert( p.id == record.id, string.format('%i %i', p.id, record.id ) )
end

test.should_return_instance_by_other_attribute = function()
  local p = Person.new()
  p.name = "Ronda"
  p:save()
  local record = Person.find{ name = "Ronda" }

  assert( p == record, string.format('%s %s', p.id, record.id) )
end

return test
