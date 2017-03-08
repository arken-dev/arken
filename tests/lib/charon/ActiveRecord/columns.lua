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

test.should_return_table = function()
  local columns = Person.columns()
  assert(type(columns) == 'table')
end

test.should_return_type = function()
  local columns = Person.columns()
  assert(columns.id.format == 'number', json.encode(columns.id))
  assert(columns.name.format == 'string', json.encode(columns.content))
  assert(columns.total.format == 'number', json.encode(columns.total))
  assert(columns.observation.format == 'string', json.encode(columns.observacao))
end

test.should_return_not_null = function()
  local columns = Person.columns()
  assert(columns.id.format == 'number', json.encode(columns.id))
  assert(columns.id.not_null == true, json.encode(columns.id))
end

return test
