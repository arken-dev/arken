local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local PersonMysql = Class.new("PersonMysql", "ActiveRecord")
local ActiveRecord = require('charon.ActiveRecord')

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS person_mysql (
    id INTEGER PRIMARY KEY AUTO_INCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
  )]]
  PersonMysql.adapter():execute(sql)
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  PersonMysql.adapter():execute([[DROP TABLE person_mysql]])
  ActiveRecord.config = nil
end

test.should_return_true_for_table_exists = function()
  assert( ActiveRecord.adapter():tableExists('person_mysql') == true )
end

test.should_return_false_for_table_not_exists = function()
  assert( ActiveRecord.adapter():tableExists('table_not_exists') == false )
end

return test
