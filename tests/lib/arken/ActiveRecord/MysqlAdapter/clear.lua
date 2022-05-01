local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local config = "config/active_record_mysql.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  Person.tableName = string.format("person_%s", os.uuid():replace('-', '_'))
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE %s (
    id INTEGER PRIMARY KEY AUTO_INCREMENT, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  ) ENGINE=InnoDB ]]
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

test.should_reset_instances_storages = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local record1 = Person.find{ id = p.id }
  assert( p == record1 , json.encode(record1) )
  ActiveRecord.clear()
  local record2 = Person.find{ id = p.id }
  assert( p ~= record2 , json.encode(record2) )
end

return test
