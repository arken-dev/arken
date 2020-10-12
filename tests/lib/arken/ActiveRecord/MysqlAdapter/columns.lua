local test   = {}
local json   = require('arken.json')
local Class  = require('arken.oop.Class')
local Adapter = require('arken.ActiveRecord.MysqlAdapter')
local Person = Class.new("Person", "ActiveRecord")
Person.tableName = string.format("person_%s", "columns") --os.uuid():replace('-', '_'))

test.beforeAll = function()
  ActiveRecord.reset()
  ActiveRecord.config = "config/active_record_mysql.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS %s (
    id INTEGER PRIMARY KEY AUTO_INCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
    created_at datetime, updated_at datetime, total REAL, cancel TINYINT,
    cancel_default_true TINYINT default 1, cancel_default_false TINYINT default 0, birthday date,
    date_meeting datetime, last_access datetime, last_hour time, sub_total double, frete float DEFAULT 0,
    observation_with_default VARCHAR(250) default 'hello !!!', total_with_default REAL DEFAULT 135.35,
    timestamp_with_default datetime default '2015-03-11 13:35:10',
    datetime_with_default datetime default '2016-05-29 14:50:30',
    date_with_default date default '2015-03-15', time_with_default time default '14:50:30'

  ) ENGINE=InnoDB]]
  --print(string.format(sql, Person.tableName))
  Person.adapter():execute(string.format(sql, Person.tableName))
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
  Person.adapter():execute(string.format("DROP TABLE IF EXISTS %s", Person.tableName))
end

test.should_return_table = function()
  local columns = Person.columns()
  assert(type(columns) == 'table')
end

test.should_return_type = function()
  local columns = Person.columns()
  assert(columns.id.format == 'number', columns.id.format)
  assert(columns.name.format == 'string', columns.name.format)
  assert(columns.total.format == 'number', columns.total.format)
  assert(columns.observation.format == 'string', columns.observation.format)
  assert(columns.created_at.format == 'datetime', columns.created_at.format)
  assert(columns.cancel.format == 'boolean', columns.cancel.format)
  assert(columns.birthday.format == 'date', columns.birthday.format)
  assert(columns.date_meeting.format == 'datetime', columns.date_meeting.format)
  assert(columns.last_access.format == 'datetime', columns.last_access.format)
  assert(columns.sub_total.format == 'number', columns.sub_total.format)
  assert(columns.frete.format == 'number', columns.frete.format)
end

test.should_return_not_null = function()
  local columns = Person.columns()
  assert(columns.id.format == 'number', json.encode(columns.id))
  assert(columns.id.notNull == true, json.encode(columns.id))
end

test.should_error_if_format_not_exists = function()
  local adapter = Adapter.new()
  local status, message = pcall(Adapter.parserFormat, adapter, 'unknow_type')
  assert( status == false )
  assert( message:contains('format_type: unknow_type not resolved') == true, message )
end

test.should_default_boolean_true = function()
  local columns = Person.columns()
  assert(columns.cancel_default_true.format == 'boolean', columns.cancel_default_true.format)
  assert(columns.cancel_default_true.default == true, tostring(columns.cancel_default_true.default))
end

test.should_default_boolean_false = function()
  local columns = Person.columns()
  assert(columns.cancel_default_false.format == 'boolean', columns.cancel_default_false.format)
  assert(columns.cancel_default_false.default == false, tostring(columns.cancel_default_false.default))
end

test.should_text_with_default = function()
  local columns = Person.columns()
  assert(columns.observation_with_default.format == 'string', columns.observation_with_default.format)
  assert(columns.observation_with_default.default == "hello !!!", tostring(columns.observation_with_default.default))
end

test.should_save_text_with_default = function()
  local value = Person.new{ name = 'John' }
  value:save()
  assert( value.observation_with_default == 'hello !!!' )
end

test.should_number_with_default = function()
  local columns = Person.columns()
  assert(columns.total_with_default.format == 'number', columns.total_with_default.format)
  assert(columns.total_with_default.default == 135.35, tostring(columns.total_with_default.default))
end

test.should_timestamp_with_default = function()
  local columns = Person.columns()
  assert(columns.timestamp_with_default.format == 'datetime', columns.timestamp_with_default.format)
  assert(columns.timestamp_with_default.default == '2015-03-11 13:35:10', tostring(columns.timestamp_with_default.default))
end

test.should_datetime_with_default = function()
  local columns = Person.columns()
  assert(columns.datetime_with_default.format == 'datetime', columns.datetime_with_default.format)
  assert(columns.datetime_with_default.default == '2016-05-29 14:50:30', tostring(columns.datetime_with_default.default))
end

test.should_date_with_default = function()
  local columns = Person.columns()
  assert(columns.date_with_default.format == 'date', columns.date_with_default.format)
  assert(columns.date_with_default.default == '2015-03-15', tostring(columns.date_with_default.default))
end

test.should_datetime_with_default = function()
  local columns = Person.columns()
  assert(columns.time_with_default.format == 'time', columns.time_with_default.format)
  assert(columns.time_with_default.default == '14:50:30', tostring(columns.time_with_default.default))
end

test.should_float_with_default = function()
  local columns = Person.columns()
  assert(columns.frete.format == 'number', columns.frete.format)
  assert(columns.frete.default == 0, tostring(columns.frete.default))
end

test.should_boolean_with_default_nil = function()
  local columns = Person.columns()
  assert(columns.cancel.format == 'boolean', columns.cancel.format)
  assert(columns.cancel.default == nil, tostring(columns.cancel.default))
end

test.should_return_primary_key_flag = function()
  local columns = Person.columns()
  assert(columns.id.primaryKey == true, 'primary key not found')
  assert(columns.name.primaryKey == false, 'name is not primary key')
end

return test
