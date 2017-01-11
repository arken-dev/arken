local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
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

test.should_return_converterd_number = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p.total = "123.45"
  assert( p.total == '123.45' )
  assert( p:read('total') == 123.45 )
end

test.should_return_converterd_number_with_comma = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p.total = "123,45"
  assert( p.total == '123,45' )
  assert( p:read('total') == 123.45 )
end

test.should_return_nil_value = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p.total = nil
  assert( p.total == nil )
  assert( p:read('total') == nil )
end

test.should_return_nil_value = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p.total = nil
  assert( p.total == nil )
  assert( p:read('total') == nil )
end

return test
