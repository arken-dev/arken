local test   = {}
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

test.define_update_with_different_value_as_created_at = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  p.name = "Rafael dos Anjos"
  p:save()

  assert(p.created_at ~= p.updated_at)
end

return test
