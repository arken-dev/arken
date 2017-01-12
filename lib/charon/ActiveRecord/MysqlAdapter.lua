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
-- CONNECT
-------------------------------------------------------------------------------

ActiveRecord_MysqlAdapter.instanceConnection = nil

function ActiveRecord_MysqlAdapter:connect()
  if ActiveRecord_MysqlAdapter.instanceConnection == nil then
    local env  = mysql.mysql()
    local password = self.password or ''
    ActiveRecord_MysqlAdapter.instanceConnection, errmsg = env:connect(self.database, self.user, password, self.host)
    if errmsg ~= nil then
      error(string.format("connect to mysql error: %s\n", errmsg))
    end
  end
  return ActiveRecord_MysqlAdapter.instanceConnection
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
  ActiveRecord_Adapter.cache[key] = record
  ActiveRecord_Adapter.neat[key]  = record:dup()
  return record
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
    for row in cursor:each({}) do
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

return ActiveRecord_MysqlAdapter
