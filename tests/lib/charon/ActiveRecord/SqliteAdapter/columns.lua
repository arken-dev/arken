local test    = {}
local json    = require('charon.json')
local Class   = require('charon.oop.Class')
local Adapter = require('charon.ActiveRecord.SqliteAdapter')
local SqliteTypes = Class.new("SqliteTypes", "ActiveRecord")
SqliteTypes.table_name = 'sqlit_adapter_types'


test.beforeAll = function()
  ActiveRecord.config = "config/active_record_sqlite.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS sqlit_adapter_types (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at timestamp, updated_at timestamp, total REAL, cancel TINYINT,
    cancel_default_true TINYINT default 1, cancel_default_false TINYINT default 0, birthday date,
    date_meeting datetime, last_access timestamp, last_hour time, sub_total double, frete float
  )]]
  SqliteTypes.adapter():execute(sql)
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

test.should_return_table = function()
  local columns = SqliteTypes.columns()
  assert(type(columns) == 'table')
end

test.should_return_type = function()
  local columns = SqliteTypes.columns()
  assert(columns.id.format == 'number', columns.id.format)
  assert(columns.name.format == 'string', columns.name.format)
  assert(columns.total.format == 'number', columns.total.format)
  assert(columns.observation.format == 'string', columns.observation.format)
  assert(columns.created_at.format == 'timestamp', columns.created_at.format)
  assert(columns.cancel.format == 'boolean', columns.cancel.format)
  assert(columns.birthday.format == 'date', columns.birthday.format)
  assert(columns.date_meeting.format == 'datetime', columns.date_meeting.format)
  assert(columns.last_access.format == 'timestamp', columns.last_access.format)
  assert(columns.sub_total.format == 'number', columns.sub_total.format)
  assert(columns.frete.format == 'number', columns.frete.format)
end

test.should_return_not_null = function()
  local columns = SqliteTypes.columns()
  assert(columns.id.format == 'number', json.encode(columns.id))
  assert(columns.id.not_null == true, json.encode(columns.id))
end

test.should_error_if_format_not_exists = function()
  local adapter = Adapter.new()
  local status, message = pcall(Adapter.parser_format, adapter, 'unknow_type')
  assert( status == false )
  assert( message:contains('format_type: unknow_type not resolved') == true, message )
end

test.should_default_true = function()
  local columns = SqliteTypes.columns()
  assert(columns.cancel_default_true.format == 'boolean', columns.cancel_default_true.format)
  assert(columns.cancel_default_true.default == true, tostring(columns.cancel_default_true.default))
end

test.should_default_false = function()
  local columns = SqliteTypes.columns()
  assert(columns.cancel_default_false.format == 'boolean', columns.cancel_default_false.format)
  assert(columns.cancel_default_false.default == false, tostring(columns.cancel_default_false.default))
end

return test
