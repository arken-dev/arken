local test    = {}
local json    = require('charon.json')
local Class   = require('charon.oop.Class')
local Adapter = require("charon.ActiveRecord.Adapter")
local Person  = Class.new("Person", "ActiveRecord")

test.beforeAll = function()
  ActiveRecord.reset()
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

test.should_validate_unique = function()
  Person.saveValidate = function(self)
    self:validate{ "unique",
      column  = 'name',
      message = 'name is used'
    }
  end
  Person.contract('save')
  local person = Person.new()
  person.name = "Aldo Junior"
  person:save()
  local person2 = Person.new()
  person2.name = "Aldo Junior"

  local status, message = person2:pcall('save')
  assert(status == false)
  assert(message.name == 'name is used', json.encode(message))
  assert( #person.errors == 0 )
end

return test
