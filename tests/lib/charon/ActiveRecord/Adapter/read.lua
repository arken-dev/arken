local test     = {}
local json     = require('charon.jsonp')
local Class    = require('charon.oop.Class')
local Employee = Class.new("EmployeeMaster", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS employee_master (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at timestamp, updated_at timestamp, total REAL, cancel TINYINT, birthday date,
    date_meeting datetime
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

test.should_return_converterd_number = function()
  local p = Employee.new()
  p.name = "Chris Weidman"
  p.total = "123.45"
  assert( p.total == '123.45' )
  assert( p:read('total') == 123.45 )
end

test.should_return_converterd_number_with_comma = function()
  local p = Employee.new()
  p.name = "Chris Weidman"
  p.total = "123,45"
  assert( p.total == '123,45' )
  assert( p:read('total') == 123.45 )
end

test.should_return_nil_value = function()
  local p = Employee.new()
  p.name = "Chris Weidman"
  p.total = nil
  assert( p.total == nil )
  assert( p:read('total') == nil )
end

test.should_return_nil_value = function()
  local p = Employee.new()
  p.name = "Chris Weidman"
  p.total = nil
  assert( p.total == nil )
  assert( p:read('total') == nil )
end

test.should_return_boolean_value = function()
  assert(Employee.columns().cancel.format == 'boolean')
  local p = Employee.new()
  p.name   = "Chris Weidman"
  p.cancel = nil
  assert( p:read('cancel') == nil )
  p.cancel = 'true'
  assert( p:read('cancel') == true )
  p.cancel = 'false'
  assert( p:read('cancel') == false )
end

test.should_return_string_value = function()
  local p = Employee.new()
  p.name  = "Chris Weidman"
  assert( p:read('name') == "Chris Weidman" )
end

test.should_return_string_timestamp_with_3_digits_in_milliconds = function()
  local p = Employee.new()
  p.name  = "Chris Weidman"
  p:save()
  ActiveRecord.clear()
  local p2 = Employee.find{ id = p.id }
  --assert( Employee.columns().created_at.format == 'time', Employee.columns().created_at.format)
  assert( p ~= p2 )
  assert( type(p.created_at) == 'string' )
  assert( type(p:read('created_at')) == 'userdata' )
  local created_at = p2.created_at:mid(1, p.created_at:indexOf('.') + 3)
  assert( p2:read('created_at'):toString() == created_at, p2:read('created_at'):toString() )
end

test.should_return_string_date = function()
  local p = Employee.new()
  p.name  = "Chris Weidman"
  p.birthday = '1975/03/21'
  p:save()
  ActiveRecord.clear()
  local p2 = Employee.find{ id = p.id }
  --assert( Employee.columns().created_at.format == 'time', Employee.columns().created_at.format)
  assert( p ~= p2 )
  assert( type(p.created_at) == 'string' )
  assert( type(p:read('birthday')) == 'userdata' )
  assert( p2:read('birthday'):toString():left(10) == p2.birthday, p2:read('birthday'):toString() )
end

test.should_return_string_datetime = function()
  local p = Employee.new()
  p.name  = "Chris Weidman"
  p.date_meeting = '2017/03/21 15:30:01'
  p:save()
  ActiveRecord.clear()
  local p2 = Employee.find{ id = p.id }
  assert( Employee.columns().date_meeting.format == 'datetime', Employee.columns().date_meeting.format)
  assert( p ~= p2 )
  assert( type(p.date_meeting) == 'string' )
  assert( type(p:read('date_meeting')) == 'userdata' )
  assert( p2:read('date_meeting'):toString():left(19) == p2.date_meeting, p2:read('date_meeting'):toString() )
end

return test
