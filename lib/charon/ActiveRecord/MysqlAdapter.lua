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

local ActiveRecord_MysqlAdapter = Class.new("ActiveRecord.MysqlAdapter", Adapter)

------------------------------------------------------------------------------
-- CONNECT
-------------------------------------------------------------------------------

ActiveRecord_MysqlAdapter.instanceConnection = nil

function ActiveRecord_MysqlAdapter:connect()
  if ActiveRecord_MysqlAdapter.instanceConnection == nil then
    local env  = mysql.mysql()
    local password = self.password or ''
    ActiveRecord_MysqlAdapter.instanceConnection, errmsg = env:connect(
      self.database, self.user, password, self.host
    )
    if errmsg ~= nil then
      ActiveRecord_MysqlAdapter.instanceConnection = nil
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
  local sql = 'INSERT INTO ' .. self.tableName .. ' '
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
      if not (column == self.primaryKey and isblank(record[self.primaryKey]))  then
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
  local sql = 'UPDATE ' .. self.tableName .. ' SET '
  local col = ''
  local key = self.tableName .. '_' .. tostring(record.id)

  if self:columns().updated_at then
    record.updated_at = self:createTimestamp()
  end

  for column, properties in pairs(self:columns()) do
    local value = record[column]
    if column ~= self.primaryKey then
      if #col > 0 then
        col = col .. ', '
      end
      col = col .. ' ' .. column ..' ' .. " = " .. self:escape(value)
    end
  end
  local result = false
  local where = ' WHERE ' .. self.primaryKey .. " = " .. self:escape(record[self.primaryKey])
  sql = sql .. col .. where
  result = self:execute(sql)
  -- neat
  local neat = Adapter.neat[record:cacheKey()] or {}
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
  record.newRecord = false
  local key = record:cacheKey()
  Adapter.cache[key] = record
  Adapter.neat[key]  = record:dup()
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
    local sql    = string.format("SHOW COLUMNS FROM %s", self.tableName)
    local result = {}
    local cursor = self:execute(sql)
    for row in cursor:each() do
      local format = self:parserFormat(row.Type)
      result[row.Field] = {
        default  = self:parserDefault(format, row.Default),
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

function ActiveRecord_MysqlAdapter:parserDefault(format, value)
  return self[format:lower() .. 'ParserDefault'](value)
end

function ActiveRecord_MysqlAdapter.stringParserDefault(value)
  return value:replaceAll("::character varying", ""):replaceChar("'", "")
end

function ActiveRecord_MysqlAdapter.timeParserDefault(value)
  return value:replaceAll("::time without time zone", ""):replaceChar("'", "")
end

function ActiveRecord_MysqlAdapter.datetimeParserDefault(value)
  return value
end

function ActiveRecord_MysqlAdapter.dateParserDefault(value)
  return value
end

function ActiveRecord_MysqlAdapter.numberParserDefault(value)
  return tonumber(value)
end

function ActiveRecord_MysqlAdapter.booleanParserDefault(value)
  return toboolean(value)
end

-------------------------------------------------------------------------------
-- PARSER FORMAT
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:parserFormat(format_type)
  format_type = format_type:lower()

  if string.contains(format_type, 'tinyint') then
    return 'boolean'
  end

  if string.contains(format_type, 'varchar') then
    return 'string'
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

  if format_type == 'float' then
    return 'number'
  end

  if string.contains(format_type, 'int') then
    return 'number'
  end

  if format_type == 'text' then
    return 'string'
  end

  if format_type == 'date' then
    return 'date'
  end

  error(string.format('format_type: %s not resolved', format_type ))
end

-------------------------------------------------------------------------------
-- TABLE EXISTS
-------------------------------------------------------------------------------

function ActiveRecord_MysqlAdapter:tableExists(tableName)
  local sql    = string.format("SHOW TABLES LIKE '%s'", tableName)
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

function ActiveRecord_MysqlAdapter:prepareMigration()
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

return ActiveRecord_MysqlAdapter
