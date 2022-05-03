local ActiveRecord = require('arken.ActiveRecord')
local Adapter = require('arken.ActiveRecord.Adapter')
local Class   = require('arken.oop.Class')
local Person  = Class.new("Person", "ActiveRecord")
local test    = {}

local config  = "config/active_record_sqlite.json"

if not os.exists(config) then
  test.config_not_exists = config
  return test
end

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
