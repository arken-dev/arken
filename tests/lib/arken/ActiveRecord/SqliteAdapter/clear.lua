local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Person = Class.new("Person", "ActiveRecord")
local Adapter = require('arken.ActiveRecord.Adapter')

local config = "config/active_record_sqlite.json"

if not os.exists( config ) then
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

test.should_reset_instances_storages = function()
  local p = Person.new()
  p.name = "Chris Weidman"
  p:save()
  local record1 = Person.find{ id = p.id }
  assert( p == record1 , json.encode(record1) )
  ActiveRecord.clear()
  local record2 = Person.find{ id = p.id }
  assert( p ~= record2 , string.format('iguais %s %s', p, record2) )
end

return test
