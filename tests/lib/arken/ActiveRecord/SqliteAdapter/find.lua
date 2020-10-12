local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.tableName = string.format('person_%s', os.uuid():replace('-', '_'))

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = string.format([[
  CREATE TABLE IF NOT EXISTS %s (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
  )]], Person.tableName)
  Person.adapter():execute(sql)
end

test.before = function()
  Person.adapter():execute([[DELETE FROM ]] .. Person.tableName)
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_return_record_stored = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local record = Person.find{ name = "Chris Weidman" }
  assert( record.name == 'Chris Weidman', json.encode(record) )
end

test.should_return_instance_by_primaryKey = function()
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

  assert( p.id == record.id, string.format('%i %i results %i', p.id, record.id, #Person.all{ name = 'Junior Cigano'} ) )
end

test.should_return_instance_by_other_attribute = function()
  local p = Person.new()
  p.name = "Junior Cigano"
  p:save()
  local record = Person.find{ name = "Junior Cigano" }

  assert( p == record, string.format('%s %s', p.id, record.id) )
end

return test
