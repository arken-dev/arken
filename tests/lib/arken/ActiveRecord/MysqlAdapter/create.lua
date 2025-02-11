local test = {}
local json  = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local config = "config/active_record_mysql.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end


test.beforeAll = function()
  Person.tableName = "person_create"
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE person_create (
    id INTEGER PRIMARY KEY AUTO_INCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  ) ENGINE=InnoDB]]
  Person.adapter():execute(sql)
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  Person.adapter():execute("DROP TABLE person_create")
  ActiveRecord.config = nil
end

test.should_insert_in_the_database = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  local result
  local cursor = Person.adapter():query([[ SELECT * FROM person_create ]])
  for row in cursor:each() do
    result = row
  end

  assert(result.name == 'Chris Weidman', json.encode(result))
end

test.define_primaryKey = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  assert(p.id > 0, json.encode(p))
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
