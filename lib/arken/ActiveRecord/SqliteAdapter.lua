-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local driver    = require "luasql.sqlite3"
local empty     = require('arken.empty')
local toboolean = require('arken.toboolean')
local Class     = require('arken.oop.Class')
local Adapter   = require('arken.ActiveRecord.Adapter')
local json      = require('arken.json')

local ActiveRecord_SqliteAdapter = Class.new("ActiveRecord.SqliteAdapter", Adapter)

------------------------------------------------------------------------------
-- CONNECT
-------------------------------------------------------------------------------

ActiveRecord_SqliteAdapter.instanceConnection = nil

function ActiveRecord_SqliteAdapter:connect()
  if ActiveRecord_SqliteAdapter.instanceConnection == nil then
    local env  = driver.sqlite3()
    ActiveRecord_SqliteAdapter.instanceConnection, errmsg = env:connect(self.database)
    if errmsg ~= nil then
      ActiveRecord_SqliteAdapter.instanceConnection = nil
      error(string.format("connect to sqlite error: %s\n", errmsg))
    end
  end
  return ActiveRecord_SqliteAdapter.instanceConnection
end

--------------------------------------------------------------------------------
-- ESCAPE
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:escape(value)
  if value == nil then
    return " NULL "
  else
    if(type(value) == 'number') then
      return tostring(value) --:replace('.', ''):replace(',', '.')
    else
      return "'" .. tostring(value):replace("'", "''") .. "'"
    end
  end
end

--------------------------------------------------------------------------------
-- INSERT
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:insert(record)
  local sql = 'INSERT INTO ' .. self.tableName .. ' '
  local col = ''
  local val = ''
  for column, value in pairs(record) do
    if not self:isReserved(column) then
    --for column, properties in pairs(self:columns(table)) do
      if not (column == self.primaryKey and empty(record[self.primaryKey]))  then
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
  return sql ..  '(' .. col .. ') VALUES (' .. val .. ')'
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:update(record)
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
      col = col .. '"' .. column ..'"' .. " = " .. self:escape(value)
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

function ActiveRecord_SqliteAdapter:create(record)
  self:bang(record)

  if self:columns().created_at then
    record.created_at = self:createTimestamp()
  end
  if self:columns().updated_at then
    record.updated_at = record.created_at or self:createTimestamp()
  end

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

--------------------------------------------------------------------------------
-- COLUMNS
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:columns()
  if self.instanceColumns == nil then
    if ActiveRecord.schema then
       self.instanceColumns = json.decode( os.read( ActiveRecord.schema .. '/' .. self.tableName .. '.json' ) )
    else
      local sql    = string.format("pragma table_info(%s)", self.tableName)
      local result = {}
      local cursor = self:execute(sql)
      -- row: cid pk type name notnull
      for row in cursor:each() do
        local format = self:parserFormat(row.type)
        result[row.name] = {
          default    = self:parserDefault(format, row.dflt_value),
          notNull    = (row.notnull == 1),
          format     = format,
          primaryKey = (row.pk == 1)
        }
      end
      self.instanceColumns = result
      cursor:close()
    end
  end
  return self.instanceColumns
end

-------------------------------------------------------------------------------
-- PARSER DEFAULT
-------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:parserDefault(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self[format .. 'ParserDefault'](value)
  end
end

function ActiveRecord_SqliteAdapter.stringParserDefault(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.timeParserDefault(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.timestampParserDefault(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.datetimeParserDefault(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.dateParserDefault(value)
  return value:mid(2, #value-2)
end

function ActiveRecord_SqliteAdapter.numberParserDefault(value)
  return tonumber(value)
end

function ActiveRecord_SqliteAdapter.booleanParserDefault(value)
  return toboolean(value)
end

-------------------------------------------------------------------------------
-- PARSER FORMAT
-------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:parserFormat(format_type)
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

function ActiveRecord_SqliteAdapter:tableExists(tableName)
  local sql    = string.format("pragma table_info(%s)", tableName)
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

-------------------------------------------------------------------------------
-- TABLES
-------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:tables()
  local list = {}
  local sql  = [[SELECT name FROM sqlite_master WHERE type='table']]

  local cursor = self:execute(sql)
  for row in cursor:each() do
    table.insert(list, row.name)
  end

  table.sort(list)

  cursor:close()
  return list
end

--------------------------------------------------------------------------------
-- SCHEMA
--------------------------------------------------------------------------------

function ActiveRecord_SqliteAdapter:schema()
  local sql  = 'CREATE TABLE IF NOT EXISTS ' .. self.tableName .. ' (\n'
  local flag = false

  for column, properties in pairs(self:columns()) do
    if flag == true then
      sql = sql .. ',\n'
    else
      flag = true
    end

    if properties.primaryKey then
      sql = sql .. column .. ' INTEGER PRIMARY KEY AUTOINCREMENT '
    else
      sql = sql .. column .. ' ' .. properties.format
    end

    if properties.notNull == true then
      sql = sql .. ' NOT NULL'
    end

    if properties.default ~= nil then
      sql = sql .. ' DEFAULT ' .. self:schemaProcessDefault(properties)
    end
  end

  sql = sql .. '\n)'

  return sql
end

function ActiveRecord_SqliteAdapter:schemaProcessDefault(properties)

  if properties.format == 'date' then
    return "'" .. properties.default .. "'"
  end

  if properties.format == 'datetime' then
    return "'" .. properties.default .. "'"
  end

  if properties.format == 'timestamp' then
    return "'" .. properties.default .. "'"
  end

  if properties.format == 'time' then
    return "'" .. properties.default .. "'"
  end

  if properties.format == 'boolean' then
    if properties.default then
      return 1
    else
      return 0
    end
  end

  if properties.format == 'string' then
    return "'" .. properties.default .. "'"
  end

  return properties.default
end

return ActiveRecord_SqliteAdapter
