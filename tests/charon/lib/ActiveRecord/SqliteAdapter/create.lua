local test = {}

local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE person (
    id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
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
  ActiveRecord.config = nil
end

test.should_insert_in_the_database = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local result
  for row in Person.adapter():connect():nrows([[ SELECT * FROM person ]]) do
    result = row
  end

  assert(result.name == 'Chris Weidman', json.encode(result))
end

test.define_primary_key = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  assert(p.id > 0, json.encode(result))
end

test.define_created_at = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  assert(type(p.created_at) == 'string', p.created_at)
end

test.define_update_at_with_the_value_of_created_at = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  assert(p.created_at == p.updated_at, p.update_at)
end

return test
