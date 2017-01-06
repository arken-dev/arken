-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local mysql     = require "luasql.mysql"
local json      = require('charon.json')
local isblank   = require('charon.isblank')
local toboolean = require('charon.toboolean')
local Class     = require('charon.oop.Class')
local Adapter   = require('charon.ActiveRecord.Adapter')

ActiveRecord_MysqlAdapter = Class.new("ActiveRecord.MysqlAdapter", Adapter)

------------------------------------------------------------------------------
-- CACHE
-------------------------------------------------------------------------------

ActiveRecord_MysqlAdapter.cache = {}
ActiveRecord_MysqlAdapter.neat  = {}

------------------------------------------------------------------------------
-- CONNECT
-------------------------------------------------------------------------------

local instanceConnection = nil

function ActiveRecord_MysqlAdapter:connect()
  if instanceConnection == nil then
    local env  = mysql.mysql()
    local password = self.password or ''
    instanceConnection, errmsg = env:connect(self.database, self.user, password)
    if errmsg ~= nil then
      error(string.format("connect to mysql error: %s\n", errmsg))
    end
  end
  return instanceConnection
end

function ActiveRecord_MysqlAdapter:close()
  if instanceConnection ~= nil then
    instanceConnection:close()
    instanceConnection = nil
  end
end

--------------------------------------------------------------------------------
-- EXECUTE
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:execute(sql)
  local time = os.microtime()
  local cursor, errmsg = self:connect():execute(sql)
  if errmsg ~= nil then
    error(string.format("error %s, sql %s", errmsg, sql))
  end
  time = os.microtime() - time
  if ActiveRecord.debug then
    print(sql .. string.format(" (%.3f) secs", time))
  end
  ActiveRecord.time = ActiveRecord.time + time
  return cursor
end

--------------------------------------------------------------------------------
-- QUERY
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:query(sql)
  local time   = os.microtime()
  local cursor = assert(self:connect():query(sql))
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

function ActiveRecord_MysqlAdapter:insert(record)
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
  return sql ..  '(' .. col .. ') VALUES (' .. val .. ') '
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:update(record)
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
      col = col .. ' ' .. column ..' ' .. " = " .. self:escape(value)
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
  local neat = ActiveRecord_MysqlAdapter.neat[record:cacheKey()] or {}
  for column, properties in pairs(self:columns()) do
    neat[column] = record[column]
  end

  return result
end

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:create(record)
  record:populate(record) -- TODO otimizar
  local sql    = self:insert(record)
  local cursor = self:execute(sql)
  record.id    = self:connect():getlastautoid()
  record.new_record = false
  local key = record:cacheKey()
  ActiveRecord_MysqlAdapter.cache[key] = record
  ActiveRecord_MysqlAdapter.neat[key]  = record:dup()
  return record
end

--------------------------------------------------------------------------------
-- DELETE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:destroy(record)
  local values = {[self.primary_key] = record[self.primary_key]} local sql = 'DELETE FROM ' .. self.table_name .. " " .. self:where(values)
  local result = self:execute(sql)
  ActiveRecord_MysqlAdapter.cache[record:cacheKey()] = false
  return result
end

--------------------------------------------------------------------------------
-- FIND
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:find(params)

  if params[self.primary_key] then
    local key = self.table_name .. '_' .. tostring(params[self.primary_key])
    if ActiveRecord_MysqlAdapter.cache[key] then
      return ActiveRecord_MysqlAdapter.cache[key]
    end
  end

  local sql = self:select(params, true)
  return self:fetch(sql)
end

--------------------------------------------------------------------------------
-- ALL
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:all(params)
  local sql    = self:select(params)
  local cursor = self:execute(sql)
  local result = {}
  for row in cursor:each() do
    table.insert(result, self:parser_fetch(row))
  end
  cursor:close()
  return result
end

function ActiveRecord_MysqlAdapter:query(sql)
  return self:execute(sql)
end

--------------------------------------------------------------------------------
-- COLUMNS
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:columns()
  if self.instanceColumns == nil then
    local sql    = string.format("SHOW COLUMNS FROM %s", self.table_name)
    local result = {}
    local cursor = self:execute(sql)
    for row in cursor:each() do
      local format = self:parser_format(row.Type)
      result[row.Field] = {
        default  = self:parser_default(format, row.Default),
        not_null = row.Null == 'NO',
        format   = format
      }
    end
    self.instanceColumns = result
    cursor:close()
  end
  return self.instanceColumns
end

-------------------------------------------------------------------------------
-- PARSER DEFAULT
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:parser_default(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self['parser_' .. format](value)
  end
end

function ActiveRecord_MysqlAdapter.parser_string(value)
  return value:replaceAll("::character varying", ""):replaceChars("'", "")
end

function ActiveRecord_MysqlAdapter.parser_time(value)
  return value:replaceAll("::time without time zone", ""):replaceChars("'", "")
end

function ActiveRecord_MysqlAdapter.parser_date(value)
  return value
end

function ActiveRecord_MysqlAdapter.parser_number(value)
  return tonumber(value)
end

function ActiveRecord_MysqlAdapter.parser_boolean(value)
  return toboolean(value)
end

-------------------------------------------------------------------------------
-- PARSER FORMAT
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:parser_format(format_type)
  format_type = format_type:lower()
  if string.contains(format_type, 'varchar') then
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

  if format_type == 'real' then
    return 'number'
  end

  if format_type == 'numeric' then
    return 'number'
  end

  if format_type == 'smallint' then
    return 'number'
  end

  if string.contains(format_type, 'int') then
    return 'number'
  end

  if format_type == 'integer' then
    return 'number'
  end

  if format_type == 'bigint' then
    return 'number'
  end

  if format_type == '' then
    return 'string'
  end

  if format_type == 'text' then
    return 'string'
  end

  if format_type == 'tinyint' then
    return 'boolean'
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

  error(string.format('format_type: %s not resolved', format_type ))
end

--------------------------------------------------------------------------------
-- FETCH
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:fetch(sql)
  local cursor = self:query(sql)
  local result = cursor:fetch({}, 'a')
  cursor:close()
  if result == nil then
    return nil
  else
    return self:parser_fetch(result)
  end
end

-------------------------------------------------------------------------------
-- PARSER FETCH
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:parser_fetch(res)
  local key  = self.table_name .. '_' .. tostring(res[self.primary_key])

  if ActiveRecord_MysqlAdapter.cache[key] then
    return ActiveRecord_MysqlAdapter.cache[key]
  else
    local neat = {}

    res.new_record = false
    for column, properties in pairs(self:columns()) do
      res[column]  = self:parser_value(properties.format, res[column])
      neat[column] = res[column]
    end

    ActiveRecord_MysqlAdapter.neat[key]  = neat
    ActiveRecord_MysqlAdapter.cache[key] = res

    return self.record_class.new(res)
  end
end

function ActiveRecord_MysqlAdapter:parser_value(format, value)
  if format == nil or value == nil then
    return nil
  else
    --print('parser_value_' .. format)
    return self['parser_value_' .. format](value)
  end
end

function ActiveRecord_MysqlAdapter.parser_value_string(value)
  return value
end

function ActiveRecord_MysqlAdapter.parser_value_time(value)
  if value == '' then
    return nil
  else
    return value
  end
end

function ActiveRecord_MysqlAdapter.parser_value_date(value)
  if value == '' then
    return nil
  else
    return value
  end
end

function ActiveRecord_MysqlAdapter.parser_value_number(value)
  value = tostring(value)
  if value:contains(',') then
    return tonumber(value:replaceChars('.', ''):replaceChars(',', '.'))
  else
    return tonumber(value)
  end
end

function ActiveRecord_MysqlAdapter.parser_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_MysqlAdapter.parser_value_timestamp(value)
  if value == '' then
    return nil
  else
    return value
  end
end

--------------------------------------------------------------------------------
-- BEGIN
--------------------------------------------------------------------------------
function ActiveRecord_MysqlAdapter:begin()
  return self:execute("BEGIN")
end

--------------------------------------------------------------------------------
-- ROLLBACK
--------------------------------------------------------------------------------
function ActiveRecord_MysqlAdapter:rollback()
  ActiveRecord_MysqlAdapter.cache = {}
  ActiveRecord_MysqlAdapter.neat  = {}
  return self:execute("ROLLBACK")
end

--------------------------------------------------------------------------------
-- COMMIT
--------------------------------------------------------------------------------
function ActiveRecord_MysqlAdapter:commit()
  ActiveRecord_MysqlAdapter.cache = {}
  ActiveRecord_MysqlAdapter.neat  = {}
  return self:execute("COMMIT")
end

--------------------------------------------------------------------------------
-- POPULATE
--------------------------------------------------------------------------------
function ActiveRecord_MysqlAdapter:populate(record, params)
  for column, properties in pairs(self:columns()) do
    if params[column] then
      record[column] = self:parser_value(properties.format, params[column])
    end
  end
end

--------------------------------------------------------------------------------
-- SET
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:set(record, column, value)
  local properties = self:columns()[column]
  record[column] = self:parser_value(properties.format, value)
end

--------------------------------------------------------------------------------
-- READ
--------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:read(record, column)
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

function ActiveRecord_MysqlAdapter:get(record, column, default)
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

function ActiveRecord_MysqlAdapter:read_value(format, value)
  if format == nil or value == nil then
    return nil
  else
    -- print('read ' .. format)
    return self['read_value_' .. format](value)
  end
end

function ActiveRecord_MysqlAdapter.read_value_string(value)
  return value
end

function ActiveRecord_MysqlAdapter.read_value_time(value)
  return value
end

function ActiveRecord_MysqlAdapter.read_value_timestamp(value)
  return value:toDateTime()
end

function ActiveRecord_MysqlAdapter.read_value_date(value)
  return value
end

function ActiveRecord_MysqlAdapter.read_value_number(value)
  if tostring(value):contains(',') then
    return tonumber(value:replaceChars('.', ''):replaceChars(',', '.'))
  else
    return tonumber(value)
  end
end

function ActiveRecord_MysqlAdapter.read_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_MysqlAdapter.read_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_MysqlAdapter.read_value_table(value)
  return table.concat(value, ',')
end

-------------------------------------------------------------------------------
-- CHANGES
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:changes(record)
  local changes = {}
  local key     = record:cacheKey()
  local neat    = ActiveRecord_MysqlAdapter.neat[key] or {}

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

function ActiveRecord_MysqlAdapter:tableExists(table_name)
  local sql  = string.format("pragma table_info(%s)", table_name)
  local flag = false

  for row in self:connect():query(sql) do
    flag = true
  end

  return flag
end

-------------------------------------------------------------------------------
-- PREPARE MIGRATIONS
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:prepareMigration()
  if not self:tableExists('schema_migration') then
    self:execute([[
      CREATE TABLE schema_migration (version VARCHAR(250) NOT NULL UNIQUE)
    ]])
  end

  local list = {}
  local sql  = "SELECT version FROM schema_migration"

  for row in self:query(sql) do
    table.insert(list, row.version)
  end

  return list
end

--------------------------------------------------------------------------------
-- COUNT
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:count(params)
  local sql = 'SELECT COUNT(*) count_all FROM ' .. self.table_name .. " " .. self:where(params)
  local count = 0
  for row in self:query(sql) do
    count = row.count_all
    break
  end
  return count
end

return ActiveRecord_MysqlAdapter
