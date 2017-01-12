-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local driver    = require "luasql.sqlite3"
local isblank   = require('charon.isblank')
local toboolean = require('charon.toboolean')
local Class     = require('charon.oop.Class')
local Adapter   = require('charon.ActiveRecord.Adapter')

ActiveRecord_SqliteAdapter = Class.new("ActiveRecord.SqliteAdapter", Adapter)

------------------------------------------------------------------------------
-- CONNECT
-------------------------------------------------------------------------------

ActiveRecord_SqliteAdapter.instanceConnection = nil

function ActiveRecord_SqliteAdapter:connect()
  if ActiveRecord_SqliteAdapter.instanceConnection == nil then
    local env  = driver.sqlite3()
    ActiveRecord_SqliteAdapter.instanceConnection, errmsg = env:connect(self.database)
    if errmsg ~= nil then
      error(string.format("connect to sqlite error: %s\n", errmsg))
    end
  end
  return ActiveRecord_SqliteAdapter.instanceConnection
end

--------------------------------------------------------------------------------
-- INSERT
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:insert(record)
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
  return sql ..  '(' .. col .. ') VALUES (' .. val .. ') '
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:update(record)
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
  local where = ' WHERE ' .. self.primary_key .. " = " .. self:escape(record[self.primary_key])
  sql = sql .. col .. where
  result = self:execute(sql)
  -- neat
  local neat = ActiveRecord_SqliteAdapter.neat[record:cacheKey()] or {}
  for column, properties in pairs(self:columns()) do
    neat[column] = record[column]
  end

  return result
end

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:create(record)
  record:populate(record) -- TODO otimizar
  local sql    = self:insert(record)
  local cursor = self:execute(sql)
  record.id    = self:connect():getlastautoid()
  record.new_record = false
  local key = record:cacheKey()
  ActiveRecord_Adapter.cache[key] = record
  ActiveRecord_Adapter.neat[key]  = record:dup()
  return record
end

--------------------------------------------------------------------------------
-- COLUMNS
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:columns()
  if self.instanceColumns == nil then
    local sql    = string.format("pragma table_info(%s)", self.table_name)
    local result = {}
    local cursor = self:execute(sql)
    for row in cursor:each() do
      local format = self:parser_format(row.type)
      result[row.name] = {
        default  = self:parser_default(format, row.dflt_value),
        not_null = row.notnull == 1,
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

function ActiveRecord_SqliteAdapter:parser_default(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self['parser_default_' .. format](value)
  end
end

function ActiveRecord_SqliteAdapter.parser_default_string(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.parser_default_time(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.parser_default_timestamp(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.parser_default_datetime(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.parser_default_date(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.parser_default_number(value)
  return tonumber(value)
end

function ActiveRecord_SqliteAdapter.parser_default_boolean(value)
  return toboolean(value)
end

-------------------------------------------------------------------------------
-- PARSER FORMAT
-------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:parser_format(format_type)
  format_type = format_type:lower()
  if string.contains(format_type, 'varchar') then
    return 'string'
  end

  if string.contains(format_type, 'timestamp') then
    return 'timestamp'
  end

  if string.contains(format_type, 'datetime') then
    return 'datetime'
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

  if format_type == 'float' then
    return 'number'
  end

  if format_type == 'integer' then
    return 'number'
  end

  if format_type == 'text' then
    return 'string'
  end

  if format_type == 'tinyint' then
    return 'boolean'
  end

  if format_type == 'date' then
    return 'date'
  end

  error('format_type: ' .. format_type ..' not resolved')
end

-------------------------------------------------------------------------------
-- TABLE EXISTS
-------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:tableExists(table_name)
  local sql    = string.format("pragma table_info(%s)", table_name)
  local cursor = self:execute(sql)
  local result = false
  if type(cursor) == 'userdata' then
    result = cursor:fetch({}, 'a') ~= nil
    cursor:close()
  end
  return result
end

-------------------------------------------------------------------------------
-- PREPARE MIGRATIONS
-------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:prepareMigration()
  if not self:tableExists('schema_migration') then
    self:execute([[
      CREATE TABLE schema_migration (version VARCHAR(250) NOT NULL UNIQUE)
    ]])
  end

  local list   = {}
  local sql    = "SELECT version FROM schema_migration"
  local cursor = self:execute(sql)
  for row in cursor:each() do
    list[row.version] = true
  end
  cursor:close()
  return list
end

return ActiveRecord_SqliteAdapter
