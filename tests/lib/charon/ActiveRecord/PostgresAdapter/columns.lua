local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.table_name = string.format("person_%s", os.uuid():replaceChars('-', '_'))

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_postgres.json"
  local sql = [[
  CREATE TABLE %s (
    id SERIAL PRIMARY KEY NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
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
  ActiveRecord.config = nil
  Person.adapter():execute(string.format("DROP TABLE %s", Person.table_name))
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
