-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local driver    = require('luasql.postgres')
local isblank   = require('charon.isblank')
local toboolean = require('charon.toboolean')
local Class     = require('charon.oop.Class')
local Adapter   = require('charon.ActiveRecord.Adapter')

ActiveRecord_PostgresAdapter = Class.new("ActiveRecord.PostgresAdapter", Adapter)

------------------------------------------------------------------------------
-- CONNECT
-------------------------------------------------------------------------------

ActiveRecord_PostgresAdapter.instanceConnection = nil

function ActiveRecord_PostgresAdapter:connect()
  if ActiveRecord_PostgresAdapter.instanceConnection == nil then
    local env = driver.postgres()
    ActiveRecord_PostgresAdapter.instanceConnection, errmsg = env:connect(
      self.database, self.user, password, self.host
    )
    if errmsg ~= nil then
      error(string.format("connect to postgres error: %s\n", errmsg))
    end
  end
  return ActiveRecord_PostgresAdapter.instanceConnection
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
  local neat = self.neat[record:cacheKey()] or {}
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
  local cursor = self:execute(sql)
  local row    = cursor:fetch({}, 'a')
  record.id    = tonumber(row[self.primary_key])
  record.new_record = false
  local key         = record:cacheKey()
  ActiveRecord_Adapter.cache[key] = record
  ActiveRecord_Adapter.neat[key]  = record:dup()
  return record
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

    local cursor = self:execute(sql)
    local result = {}
    for row in cursor:each({}) do
      local format = self:parser_format(row.format_type)
      result[row.attname] = {
        default  = self:parser_default(format, row.adsrc),
        not_null = toboolean(row.attnotnull),
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

function ActiveRecord_PostgresAdapter:parser_default(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self['parser_' .. format](value)
  end
end

function ActiveRecord_PostgresAdapter.parser_string(value)
  return value:replaceAll("::character varying", ""):replaceChars("'", "")
end

function ActiveRecord_PostgresAdapter.parser_time(value)
  return value:replaceAll("::time without time zone", ""):replaceChars("'", "")
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

  if format_type == 'real' then
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

return ActiveRecord_PostgresAdapter
