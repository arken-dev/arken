local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local PersonPostgres = Class.new("PersonPostgres", "ActiveRecord")
local ActiveRecord = require('charon.ActiveRecord')

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_postgres.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS person_postgres (
    id SERIAL PRIMARY KEY NOT NULL, name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT, total REAL
  )]]
  PersonPostgres.adapter():execute(sql)
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  PersonPostgres.adapter():execute([[DROP TABLE IF EXISTS person_postgres]])
  ActiveRecord.config = nil
end

test.should_return_true_for_table_exists = function()
  assert( PersonPostgres.adapter():tableExists('person_postgres') == true )
end

test.should_return_false_for_table_not_exists = function()
  assert( PersonPostgres.adapter():tableExists('table_not_exists') == false )
end

return test
