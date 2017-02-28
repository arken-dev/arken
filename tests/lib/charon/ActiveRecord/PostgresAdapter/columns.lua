local test   = {}
local json   = require('charon.json')
local Class  = require('charon.oop.Class')
local Adapter = require('charon.ActiveRecord.PostgresAdapter')
local Person = Class.new("Person", "ActiveRecord")
Person.table_name = string.format("person_%s", "columns") --os.uuid():replaceChar('-', '_'))

test.beforeAll = function()
  ActiveRecord.config = "config/active_record_postgres.json"
  local sql = [[
  CREATE TABLE IF NOT EXISTS %s (
    id SERIAL PRIMARY KEY NOT NULL, name VARCHAR(250), observation TEXT,
    created_at timestamp, updated_at timestamp, total REAL, cancel boolean,
    cancel_default_true boolean default true, cancel_default_false boolean default false, birthday date,
    date_meeting timestamp, last_access timestamp, last_hour time, sub_total float, frete float DEFAULT 0,
    observation_with_default VARCHAR(250) default 'hello !!!', total_with_default REAL DEFAULT 135.35,
    timestamp_with_default timestamp default '2015-03-11 13:35:10',
    datetime_with_default timestamp default '2016-05-29 14:50:30',
    date_with_default date default '2015-03-15', time_with_default time default '14:50:30',
    bytea_type bytea, tsvector_type tsvector, total_numeric numeric(9,2), total_bigint bigint,
    total_smallint smallint
  )]]
  --print(string.format(sql, Person.table_name))
  Person.adapter(true):execute(string.format(sql, Person.table_name))
end

test.before = function()
  ActiveRecord.begin()
end

test.after = function()
  ActiveRecord.rollback()
end

test.afterAll = function()
  ActiveRecord.config = nil
  Person.adapter():execute(string.format("DROP TABLE IF EXISTS %s", Person.table_name))
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
  assert(columns.id.not_null == true, json.encode(columns.id))
end

test.should_error_if_format_not_exists = function()
  local adapter = Adapter.new()
  local status, message = pcall(Adapter.parser_format, adapter, 'unknow_type')
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

test.should_bytea_with_default_nil = function()
  local columns = Person.columns()
  assert(columns.bytea_type.format == 'string', columns.bytea_type.format)
  assert(columns.bytea_type.default == nil, tostring(columns.bytea_type.default))
end

test.should_tsvector_with_default_nil = function()
  local columns = Person.columns()
  assert(columns.tsvector_type.format == 'string', columns.tsvector_type.format)
  assert(columns.tsvector_type.default == nil, tostring(columns.tsvector_type.default))
end

test.should_numeric_with_default_nil = function()
  local columns = Person.columns()
  assert(columns.total_numeric.format == 'number', columns.total_numeric.format)
  assert(columns.total_numeric.default == nil, tostring(columns.total_numeric.default))
end

test.should_bigint_with_default_nil = function()
  local columns = Person.columns()
  assert(columns.total_bigint.format == 'number', columns.total_bigint.format)
  assert(columns.total_bigint.default == nil, tostring(columns.total_bigint.default))
end

test.should_smallint_with_default_nil = function()
  local columns = Person.columns()
  assert(columns.total_smallint.format == 'number', columns.total_smallint.format)
  assert(columns.total_smallint.default == nil, tostring(columns.total_smallint.default))
end


return test
