local ActiveRecord = require('charon.ActiveRecord')
local Adapter = require('charon.ActiveRecord.Adapter')
local Class   = require('charon.oop.Class')
local Person  = Class.new("Person", "ActiveRecord")
local test    = {}

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


test.should_return_debug_if_true = function()
  local adapter = Person.adapter()
  local value
  Adapter.output = function(str)
    value = str
  end
  ActiveRecord.debug = true
  local cursor  = adapter:execute('SELECT * FROM person')
  ActiveRecord.debug = false
  Adapter.output = print
  assert( value:contains('SELECT * FROM person'), value )
end

return test
