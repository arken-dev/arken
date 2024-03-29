local test   = {}
local Class  = require('arken.oop.Class')
local Departament = Class.new("Departament", "ActiveRecord")
local Employee    = Class.new("Employee", "ActiveRecord")

Employee.belongsTo {
  name   = 'departament',
  record = 'Departament',
  foreignKey = 'departament_id'
}

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

test.should_return_departament_instance = function()
  local departament = Departament.new{ description = 'Directory' }
  departament:save()
  local employee = Employee.new{ name = "John", departament_id = departament.id }
  employee:save()
  assert( employee:departament() == departament )
end

test.should_return_departament_description = function()
  local departament = Departament.new{ description = 'Directory' }
  departament:save()
  local employee = Employee.new{ name = "John", departament_id = departament.id }
  employee:save()
  assert( employee:departament().description == 'Directory', employee:departament().description  )
end

test.should_return_nil_if_departament_id_is_nil = function()
  local employee = Employee.new{ name = "John", departament_id = nil }
  employee:save()
  assert( employee:departament() == nil )
end

return test
