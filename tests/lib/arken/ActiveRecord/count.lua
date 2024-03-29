local test   = {}
local Class  = require('arken.oop.Class')
local Departament = Class.new("Departament", "ActiveRecord")
local Employee    = Class.new("Employee", "ActiveRecord")

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

Employee.belongsTo {
  name   = 'departament',
  record = 'Departament',
  foreign_key = 'departament_id'
}

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
  ActiveRecord.adapter():execute(sql)
  local sql = [[
  CREATE TABLE IF NOT EXISTS departament (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    description VARCHAR(250),
    created_at TEXT, updated_at TEXT
  )]]
  Departament.adapter():execute(sql)
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

test.should_return_1_for_all = function()
  local departament = Departament.new{ description = 'Directory' }
  departament:save()
  local employee = Employee.new{ name = "John", departament_id = departament.id }
  employee:save()
  assert( Employee.count() == 1 )
end

test.should_return_2_for_all = function()
  local john = Employee.new{ name = "John" }
  john:save()

  local steve = Employee.new{ name = "Steve" }
  steve:save()

  assert( Employee.count() == 2 )
end

test.should_return_1_for_departament = function()
  local departament = Departament.new{ description = 'Directory' }
  departament:save()

  local john = Employee.new{ name = "John", departament_id = departament.id }
  john:save()

  local steve = Employee.new{ name = "Steve", departament_id = nil }
  steve:save()

  assert( Employee.count{ departament_id = departament.id } == 1 )
end

return test
