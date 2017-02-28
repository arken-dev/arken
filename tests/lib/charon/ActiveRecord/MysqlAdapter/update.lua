local test   = {}
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.table_name = string.format("person_%s", "update") --os.uuid():replaceChar('-', '_'))

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE %s (
    id INTEGER PRIMARY KEY AUTO_INCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  ) ENGINE=InnoDB ]]
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

test.define_update_with_different_value_as_created_at = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  p.name = "Rafael dos Anjos"
  p:save()

  assert(p.created_at ~= p.updated_at)
end

return test
