local test   = {}
local Class  = require('arken.oop.Class')
local Employee = Class.new("Employee", "ActiveRecord")
local Login    = Class.new("Login", "ActiveRecord")

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
  test.config_not_exists = config
  return test
end

Employee.hasOne {
  name   = 'login',
  record = 'Login',
  foreignKey = 'employee_id'
}

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS employee (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    departament_id INTEGER, total REAL,
    name VARCHAR(250), observation TEXT,
    created_at TEXT, updated_at TEXT
  )]]
  ActiveRecord.adapter():execute(sql)
  local sql = [[
  CREATE TABLE IF NOT EXISTS login (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    login VARCHAR(250),
    employee_id INTEGER,
    created_at TEXT, updated_at TEXT
  )]]
  Login.adapter():execute(sql)
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

test.should_return_login_instance = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  local login = Login.new{ login = 'john', employee_id = employee.id }
  login:save()
  assert( employee:login() == login )
end

test.should_return_login_description = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  local login = Login.new{ login = 'john', employee_id = employee.id }
  login:save()
  assert( employee:login().login == 'john' )
end

test.should_return_nil_if_whitout_login = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  assert( employee:login() == nil )
end

return test
