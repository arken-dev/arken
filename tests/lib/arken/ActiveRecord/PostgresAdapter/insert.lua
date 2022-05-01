local test = {}
local json  = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local config  = "config/active_record_postgres.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  Person.tableName = string.format("person_%s", "insert")
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_postgres.json"
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
  local insert = string.format([[INSERT INTO %s (name, observation) VALUES ('I am table', 'simple test') RETURNING id]], Person.tableName)
  assert( sql == insert, sql )
end

test.should_create_insert_maintain_case = function()
  local sql = Person.adapter():insert({ Name = "I am table", Observation = "simple test" })
  local insert = string.format([[INSERT INTO %s (Name, Observation) VALUES ('I am table', 'simple test') RETURNING id]], Person.tableName)
  assert( sql == insert, sql )
end

return test
