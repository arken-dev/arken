local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local ActiveRecord = require('charon.ActiveRecord')

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS person (
    id INTEGER PRIMARY KEY AUTO_INCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
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
  Person.adapter():execute([[DROP TABLE person]])
  ActiveRecord.config = nil
end

test.should_return_true_for_table_exists = function()
  assert( ActiveRecord.adapter():tableExists('person') == true )
end

test.should_return_false_for_table_not_exists = function()
  assert( ActiveRecord.adapter():tableExists('table_not_exists') == false )
end

return test
