local test = {}
local json  = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
Person.tableName = string.format("person_%s", os.uuid():replace('-', '_'))

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE %s (
    id SERIAL PRIMARY KEY, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  )]]
  Person.adapter():execute(string.format(sql, Person.tableName))
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  Person.adapter():execute(string.format("DROP TABLE %s", Person.tableName))
  ActiveRecord.config = nil
end

test.should_create_insert = function()
  local sql = Person.adapter():insert({ name = "I am table", observation = "simple test" })
  local insert = string.format([[INSERT INTO %s (name, observation) VALUES ('I am table', 'simple test')]], Person.tableName)
  assert( sql == insert, sql )
end

return test
