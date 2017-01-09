local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Person = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS person (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
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
  ActiveRecord.config = nil
end

test.should_execute_validate = function()
  Person.saveValidate = function()
    error('do not save !')
  end
  Person.contract('save')

  local person = Person.new()
  person.name = "Chris Weidman"
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message:contains('do not save !') == true )
end

test.should_execute_validate_with_ = function()
  Person.saveValidate = function(self)
    self:validate{ 'presence', column = 'name', message = 'name is required' }
  end
  Person.contract('save')

  local person = Person.new()
  person.name = ''
  local status, message = person:pcall('save')
  assert(status == false)
  assert(message.name == 'name is required', message.name )
end

return test
