-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

require 'pgsql'

local isblank   = require('charon.isblank')
local toboolean = require('charon.toboolean')
local Class     = require('charon.oop.Class')

ActiveRecord_PostgresAdapter = Class.new("ActiveRecord.PostgresAdapter", "ActiveRecord.Adapter")

------------------------------------------------------------------------------
-- CACHE
-------------------------------------------------------------------------------

ActiveRecord_PostgresAdapter.cache = {}
ActiveRecord_PostgresAdapter.neat  = {}

------------------------------------------------------------------------------
-- CONNECT
-------------------------------------------------------------------------------

local instanceConnection = nil

function ActiveRecord_PostgresAdapter:connect()
  if instanceConnection == nil then
    local config = ""
    local config = config .. ' dbname=' .. self.database
          config = config .. ' user=' .. self.user
          config = config .. ' password=' .. self.password
          config = config .. ' host=' .. self.host
    instanceConnection = pg.connect(config)
  end
  return instanceConnection
end

--------------------------------------------------------------------------------
-- EXECUTE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:execute(sql)
  local time = os.microtime()
  local result = assert(self:connect():exec(sql))
  time = os.microtime() - time
  if ActiveRecord.debug then
    print(sql .. string.format(" (%.3f) secs", time))
  end
  ActiveRecord.time = ActiveRecord.time + time
  return result
end

--------------------------------------------------------------------------------
-- QUERY
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:query(sql)
  local time = os.microtime()
  local result = assert(self:connect():query(sql))
  time = os.microtime() - time
  if ActiveRecord.debug then
    --print(sql .. string.format(" (%.3f) secs", time))
  end
  ActiveRecord.time = ActiveRecord.time + time
  return result
end

--------------------------------------------------------------------------------
-- INSERT
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:insert(record)
  self:bang(record)
  local sql = 'INSERT INTO ' .. self.table_name .. ' '
  local col = ''
  local val = ''
  if self:columns().created_at then
    record.created_at = self:createTimestamp()
  end
  if self:columns().updated_at then
    record.updated_at = record.created_at
  end
  if self:columns().uuid then
    record.uuid = os.uuid()
  end
  for column, value in pairs(record) do
    if not self:isReserved(column) then
    --for column, properties in pairs(self:columns(table)) do
      if not (column == self.primary_key and isblank(record[self.primary_key]))  then
        --local value = record[column]
        if #col > 0 then
          col = col .. ', '
        end
        col = col .. column

        if #val > 0 then
          val = val .. ', '
        end
        val = val .. self:escape(value)
      end
    end
  end

  if col:len() == 0 then
    for column, properties in pairs(self:columns(table)) do
      if column ~= self.primary_key then
        if col:len() > 0 then
          col = col .. ', '
        end
        if val:len() > 0 then
          val = val .. ', '
        end
        col = col .. column
        val = val .. ' NULL '
      end
    end
  end
  return sql ..  '(' .. col .. ') VALUES (' .. val .. ') ' .. ' RETURNING ' .. record.primary_key
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:update(record)
  self:bang(record)
  local sql = 'UPDATE ' .. self.table_name .. ' SET '
  local col = ''
  local key = self.table_name .. '_' .. tostring(record.id)

  if self:columns().updated_at then
    record.updated_at = self:createTimestamp()
  end

  for column, properties in pairs(self:columns()) do
    local value = record[column]
    if column ~= self.primary_key then
      if #col > 0 then
        col = col .. ', '
      end
      col = col .. '"' .. column ..'"' .. " = " .. self:escape(value)
    end
  end
  local result = false
  if col:len() > 0 then
    local where = ' WHERE ' .. self.primary_key .. " = " .. self:escape(record[self.primary_key])
    sql = sql .. col .. where
    result = self:execute(sql)
  else
    result = true
  end
  -- neat
  local neat = ActiveRecord_PostgresAdapter.neat[record:cacheKey()] or {}
  for column, properties in pairs(self:columns()) do
    neat[column] = record[column]
  end

  return result
end

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:create(record)
  record:populate(record) -- TODO otimizar
  local sql    = self:insert(record)
  local status, result = pcall(self.execute, self, sql)
  if status == false then
    error(string.format("error %s, sql %s", result, sql))
  else
    local cursor = result
    local row    = cursor:fetch({}, 'a')
    record.id    = tonumber(row[self.primary_key])
    record.new_record = false
    local key         = record:cacheKey()
    self.cache[key]   = record
    ActiveRecord_PostgresAdapter.neat[key] = record:dup()
  end
  return record
end

--------------------------------------------------------------------------------
-- DELETE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:destroy(record)
  local values = {[self.primary_key] = record[self.primary_key]}
  local sql = 'DELETE FROM ' .. self.table_name .. " " .. self:where(values)
  local result = self:execute(sql)
  self.cache[record:cacheKey()] = false
  return result
end

--------------------------------------------------------------------------------
-- FIND
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:find(params)

  if params[self.primary_key] then
    local key = self.table_name .. '_' .. tostring(params[self.primary_key])
    if self.cache[key] then
      return self.cache[key]
    end
  end

  local sql  = self:select(params, true)
  local data = self:fetch(sql)
  if data == nil then
    return nil
  else
    data.new_record = false
    data = self.record_class.new(data)
    local key = self.table_name .. '_' .. tostring(data[self.primary_key])
    return data
  end

end

--------------------------------------------------------------------------------
-- ALL
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:all(params)
  local sql    = self:select(params)
  local res    = self:execute(sql)
  local result = {}
  for row in res:rows() do
    table.insert(result, self:parser_fetch(row))
  end
  return result
end

--------------------------------------------------------------------------------
-- COLUMNS
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:columns()
  if self.instanceColumns == nil then
    sql = [[
      SELECT a.attname, format_type(a.atttypid, a.atttypmod), d.adsrc, a.attnotnull
        FROM pg_attribute a LEFT JOIN pg_attrdef d
          ON a.attrelid = d.adrelid AND a.attnum = d.adnum
        WHERE a.attrelid = ']] .. self.table_name .. [['::regclass
          AND a.attnum > 0 AND NOT a.attisdropped
        ORDER BY a.attnum
    ]]

    local res    = self:execute(sql)
    local result = {}
    for row in res:rows() do
      local format = self:parser_format(row.format_type)
      result[row.attname] = {
        default  = self:parser_default(format, row.adsrc),
        not_null = toboolean(row.attnotnull),
        format   = format
      }
    end
    self.instanceColumns = result
  end
  return self.instanceColumns

end

-------------------------------------------------------------------------------
-- PARSER DEFAULT
-------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:parser_default(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self['parser_' .. format](value)
  end
end

function ActiveRecord_PostgresAdapter.parser_string(value)
  return value:swap("::character varying", ""):swap("'", "")
end

function ActiveRecord_PostgresAdapter.parser_time(value)
  return value:swap("::time without time zone", ""):swap("'", "")
end

function ActiveRecord_PostgresAdapter.parser_date(value)
  return value
end

function ActiveRecord_PostgresAdapter.parser_number(value)
  return tonumber(value)
end

function ActiveRecord_PostgresAdapter.parser_boolean(value)
  return toboolean(value)
end

-------------------------------------------------------------------------------
-- PARSER FORMAT
-------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:parser_format(format_type)
  if string.contains(format_type, 'character') then
    return 'string'
  end

  if string.contains(format_type, 'timestamp') then
    return 'timestamp'
  end

  if string.contains(format_type, 'time') then
    return 'time'
  end

  if string.contains(format_type, 'double') then
    return 'number'
  end

  if string.contains(format_type, 'numeric') then
    return 'number'
  end

  if format_type == 'smallint' then
    return 'number'
  end

  if format_type == 'integer' then
    return 'number'
  end

  if format_type == 'bigint' then
    return 'number'
  end

  if format_type == 'text' then
    return 'string'
  end

  if format_type == 'boolean' then
    return 'boolean'
  end

  if format_type == 'date' then
    return 'date'
  end

  if format_type == 'bytea' then
    return 'bytea'
  end

  if format_type == 'tsvector' then
    return 'string'
  end

  error('format_type: ' .. format_type ..' not resolved')
end

--------------------------------------------------------------------------------
-- FETCH
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:fetch(sql)
  local result = self:execute(sql)
  local res    = result:fetch({}, 'a')
  if res == nil then
    return nil
  else
    return self:parser_fetch(res)
  end
end

-------------------------------------------------------------------------------
-- PARSER FETCH
-------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:parser_fetch(res)
  local key  = self.table_name .. '_' .. tostring(res[self.primary_key])
  if ActiveRecord_PostgresAdapter.cache[key] then
    return ActiveRecord_PostgresAdapter.cache[key]
  else
    local neat = {}

    res.new_record = false
    for column, properties in pairs(self:columns()) do
      res[column]  = self:parser_value(properties.format, res[column])
      neat[column] = res[column]
    end

    ActiveRecord_PostgresAdapter.neat[key]  = neat
    ActiveRecord_PostgresAdapter.cache[key] = res

    return self.record_class.new(res)
  end
end

function ActiveRecord_PostgresAdapter:parser_value(format, value)
  if format == nil or value == nil then
    return nil
  else
    --print('parser_value_' .. format)
    return self['parser_value_' .. format](value)
  end
end

function ActiveRecord_PostgresAdapter.parser_value_string(value)
  return value
end

function ActiveRecord_PostgresAdapter.parser_value_time(value)
  if value == '' then
    return nil
  else
    return value
  end
end

function ActiveRecord_PostgresAdapter.parser_value_date(value)
  if value == '' then
    return nil
  else
    return value
  end
end

function ActiveRecord_PostgresAdapter.parser_value_number(value)
  value = tostring(value)
  if value:contains(',') then
    return tonumber(value:replace('.', ''):replace(',', '.'))
  else
    return tonumber(value)
  end
end

function ActiveRecord_PostgresAdapter.parser_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_PostgresAdapter.parser_value_timestamp(value)
  if value == '' then
    return nil
  else
    return value
  end
end

--------------------------------------------------------------------------------
-- BEGIN
--------------------------------------------------------------------------------
function ActiveRecord_PostgresAdapter:begin()
  return self:execute("BEGIN")
end

--------------------------------------------------------------------------------
-- ROLLBACK
--------------------------------------------------------------------------------
function ActiveRecord_PostgresAdapter:rollback()
  ActiveRecord_PostgresAdapter.cache = {}
  ActiveRecord_PostgresAdapter.neat  = {}
  return self:execute("ROLLBACK")
end

--------------------------------------------------------------------------------
-- COMMIT
--------------------------------------------------------------------------------
function ActiveRecord_PostgresAdapter:commit()
  ActiveRecord_PostgresAdapter.cache = {}
  ActiveRecord_PostgresAdapter.neat  = {}
  return self:execute("COMMIT")
end

--------------------------------------------------------------------------------
-- POPULATE
--------------------------------------------------------------------------------
function ActiveRecord_PostgresAdapter:populate(record, params)
  for column, properties in pairs(self:columns()) do
    if params[column] then
      record[column] = self:parser_value(properties.format, params[column])
    end
  end
end

--------------------------------------------------------------------------------
-- SET
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:set(record, column, value)
  local properties = self:columns()[column]
  record[column] = self:parser_value(properties.format, value)
end

--------------------------------------------------------------------------------
-- READ
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:read(record, column)
  local value = record[column]
  column = self:columns()[column]
  if value == nil or column == nil then
    return nil
  else
    return self:read_value(column.format, value)
  end
end

--------------------------------------------------------------------------------
-- GET
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:get(record, column, default)
  local value = record[column]
  column = self:columns()[column]
  if value == nil or column == nil then
    return default
  else
    return self:read_value(column.format, value)
  end
end

--------------------------------------------------------------------------------
-- READ VALUE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:read_value(format, value)
  if format == nil or value == nil then
    return nil
  else
    -- print('read ' .. format)
    return self['read_value_' .. format](value)
  end
end

function ActiveRecord_PostgresAdapter.read_value_string(value)
  return value
end

function ActiveRecord_PostgresAdapter.read_value_time(value)
  return value
end

function ActiveRecord_PostgresAdapter.read_value_timestamp(value)
  return value:toDateTime()
end

function ActiveRecord_PostgresAdapter.read_value_date(value)
  return value
end

function ActiveRecord_PostgresAdapter.read_value_number(value)
  if tostring(value):contains(',') then
    return tonumber(value:replace('.', ''):replace(',', '.'))
  else
    return tonumber(value)
  end
end

function ActiveRecord_PostgresAdapter.read_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_PostgresAdapter.read_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_PostgresAdapter.read_value_table(value)
  return table.concat(value, ',')
end

-------------------------------------------------------------------------------
-- CHANGES
-------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:changes(record)
  local changes = {}
  local key     = record:cacheKey()
  local neat    = ActiveRecord_PostgresAdapter.neat[key] or {}

  for column, properties in pairs(self:columns()) do
    if record[column] ~= neat[column] then
      changes[column] = { neat[column], record[column] }
    end
  end

  return changes
end

-------------------------------------------------------------------------------
-- TABLE EXISTS
-------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:tableExists(table_name)

  local flag = false
  local sql  = string.format([[
    SELECT 1
    FROM   pg_catalog.pg_class c
    JOIN   pg_catalog.pg_namespace n ON n.oid = c.relnamespace
    WHERE  c.relname = '%s']], table_name)
  local res = self:execute(sql)
  for row in res:rows() do
    flag = true
    break
  end

  return flag
end

-------------------------------------------------------------------------------
-- PREPARE MIGRATIONS
-------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:prepareMigration()
  if not self:tableExists('schema_migration') then
    self:execute([[
      CREATE TABLE schema_migration (version VARCHAR(250) NOT NULL UNIQUE)
    ]])
  end

  local list = {}
  local sql  = "SELECT version FROM schema_migration"
  local res = self:execute(sql)
  for row in res:rows() do
    table.insert(list, row.version)
  end

  return list
end

--------------------------------------------------------------------------------
-- COUNT
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:count(params)
  local sql   = 'SELECT COUNT(*) count_all FROM ' .. self.table_name .. " " .. self:where(params)
  local res   = self:execute(sql)
  local count = 0
  for row in res:rows() do
    count = row.count_all
    break
  end
  return count
end

return ActiveRecord_PostgresAdapter
