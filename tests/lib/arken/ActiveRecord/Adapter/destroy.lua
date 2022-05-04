local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")

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

test.should_return_nil_in_next_find = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  p:destroy()

  local record = Person.find{ name = "Chris Weidman" }
  assert( record == nil, json.encode(record) )
end

test.should_error_if_save = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  p:destroy()

  local status, message = p:pcall('save')

  assert( status == false )
  assert( message:contains( 'record destroyed' ), message )
end

test.should_error_if_update = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()

  p:destroy()

  local status, message = p:pcall('update')

  assert( status == false )
  assert( message:contains( 'record destroyed' ), message )
end

return test
