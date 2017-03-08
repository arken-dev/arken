local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS person (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
  )]]
  Person.adapter():execute(sql)
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

test.should_return_count_with_by_name = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local count = Person.count{ name = "Chris Weidman" }
  assert( count == 1, count )
end

test.should_return_count_with_by_whithout_params = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local count = Person.count()
  assert( count == 1, count )
end

test.should_return_zero_with_by_name_invalid = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local count = Person.count{ name = "Junior Cigano" }
  assert( count == 0, count )
end

return test
