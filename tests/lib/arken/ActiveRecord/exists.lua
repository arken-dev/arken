local test     = {}
local Class    = require('arken.oop.Class')
local Employee = Class.new("Employee", "ActiveRecord")

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS employee (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    name VARCHAR(250), observation TEXT,
    departament_id INTEGER, total REAL,
    created_at TEXT, updated_at TEXT
  )]]
  Employee.adapter():execute(sql)
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
end

test.should_return_true = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  assert( Employee.exists{ name = "John" } == true )
end

test.should_return_true = function()
  assert( Employee.exists{ name = "Norman" } == false )
end

return test
