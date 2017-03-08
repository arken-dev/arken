local test   = {}
local Class        = require('charon.oop.Class')
local ActiveRecord = require('charon.ActiveRecord')
local Employee = Class.new("Employee", "ActiveRecord")
local Task     = Class.new("Task", "ActiveRecord")

Employee.hasMany {
  name   = 'tasks',
  record = 'Task',
  foreignKey = 'employee_id'
}

Task.belongsTo {
  name   = 'employee',
  record = 'Employee',
  foreignKey = 'employee_id'
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
  CREATE TABLE IF NOT EXISTS task (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    description VARCHAR(250),
    employee_id INTEGER
  )]]
  Task.adapter():execute(sql)
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

test.should_return_employee_instance = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  local task1 = Task.new{ description = 'task 1', employee_id = employee.id }
  task1:save()
  local task2 = Task.new{ description = 'task 2', employee_id = employee.id }
  task2:save()
  assert( task1:employee() == employee )
  assert( task2:employee() == employee )
end

test.should_number_of_tasks = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  local task1 = Task.new{ description = 'task 1', employee_id = employee.id }
  task1:save()
  local task2 = Task.new{ description = 'task 2', employee_id = employee.id }
  task2:save()
  assert( #employee:tasks() == 2 )
end

test.should_return_empty_table = function()
  local employee = Employee.new{ name = "John" }
  assert( #employee:tasks() == 0 )
end

test.should_return_order_asc_and_desc = function()
  local employee = Employee.new{ name = "John" }
  employee:save()
  local task1 = Task.new{ description = 'task 1', employee_id = employee.id }
  task1:save()
  local task2 = Task.new{ description = 'task 2', employee_id = employee.id }
  task2:save()
  Employee.hasMany {
    name   = 'tasks',
    record = 'Task',
    foreignKey = 'employee_id',
    order = 'description DESC'
  }

  assert( employee:tasks()[1].description == 'task 2', employee:tasks()[1].description )

  Employee.hasMany {
    name   = 'tasks',
    record = 'Task',
    foreignKey = 'employee_id',
    order = 'description ASC'
  }

  assert( employee:tasks()[1].description == 'task 1', employee:tasks()[1].description )
end

return test
