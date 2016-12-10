local test   = {}
local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  local sql = [[
  CREATE TABLE person (
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
  Person.adapter():execute("DROP TABLE person")
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
