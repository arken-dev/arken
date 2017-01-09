-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Array = require('charon.Array')
local Class = require('charon.oop.Class')

ActiveRecord_Adapter = Class.new("ActiveRecord_Adapter")

ActiveRecord_Adapter.reserved = {'new_record', 'class', 'errors'}

ActiveRecord_Adapter.errors = {}
ActiveRecord_Adapter.cache  = {}
ActiveRecord_Adapter.neat   = {}

-------------------------------------------------------------------------------
-- FORMAT
-------------------------------------------------------------------------------

local format = {}

format.boolean = function(value)
  return "'".. tostring(value) .. "'"
end

format.number = function(value)
  return tostring(value):replaceChars('.', ''):replaceChars(',', '.')
end

format.string = function(value)
  return "'".. value .. "'"
end

format.table = function(value)
  return table.concat(value, ',')
end

format.userdata = function(value)
  return "'" .. value:__tostring() .. "'"
end

--------------------------------------------------------------------------------
-- FINDERS
--------------------------------------------------------------------------------
ActiveRecord_Adapter.finders = {}

ActiveRecord_Adapter.finders['string'] = function(value)
  if value == 'NULL' or value == 'NOT NULL' then
    return ' IS ' .. value
  else
    return " = '".. value .. "'"
  end
end

ActiveRecord_Adapter.finders['boolean'] = function(value)
  return " = '".. tostring(value) .. "'"
end

ActiveRecord_Adapter.finders['number'] = function(value)
  return " = " .. tostring(value):replaceChars('.', ''):replaceChars(',', '.')
end

ActiveRecord_Adapter.finders['table'] = function(value)
  return " = '".. value['__finder'](value) .. "'"
end

ActiveRecord_Adapter.finders['userdata'] = function(value)
  return " = '".. value['__finder'](value) .. "'"
end

--------------------------------------------------------------------------------
-- DEFAULT VALUES
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:defaultValues(record)
  record.new_record = true
  for column, properties in pairs(self:columns()) do
    if record[column] == nil then -- and properties.not_null then
      record[column] = properties.default
    end
  end
end

--------------------------------------------------------------------------------
-- ESCAPE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:escape(value)
  if value == nil then
    return " NULL "
  else
    if(type(value) == 'number') then
      return tostring(value) --:replaceChars('.', ''):replaceChars(',', '.')
    else
      return "'" .. tostring(value) .. "'"
    end
  end
end

--------------------------------------------------------------------------------
-- isReserved
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:isReserved(column)
  for _, value in ipairs(ActiveRecord_Adapter.reserved) do
    if value == column then
      return true
    end
  end
  return false
end

--------------------------------------------------------------------------------
-- WHERE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:where(values, flag)

  local result = ""
  local col    = ""
  local join   = values.join
  local order  = values.order
  local limit  = values.limit

  values.join    = nil
  values.binding = nil
  values.order   = nil
  values.limit   = nil

  if type(join) == 'table' then
    local tmp = join[1]
    for index, value in pairs(join) do
      tmp = string.replaceAll(tmp, '$' .. index, format[type(value)](value))
    end
  join = tmp
  end

  if values.where then
    local where = values.where
    if where ~= '' then
      where = ' WHERE ' .. where
      for index, value in pairs(values) do
        where = string.replaceAll(where, '$' .. index, format[type(value)](value))
      end
      result = where
    end
  else
    for index, value in pairs(values) do
      if ActiveRecord_Adapter.reserved[index] == nil then
        if #col > 0 then
          col = col .. ' AND '
        end
        col = col .. index .. self.finders[type(value)](value)
      end
    end
    if flag and #col == 0 then
      error "parameters for find empty"
    end
    if col ~= '' then
      result = ' WHERE ' .. result .. col
    end
  end
  if join then
    result = join .. result
  end
  if order then
    result = result .. ' ORDER BY ' .. order
  end
  if limit then
    result = result .. ' LIMIT ' .. limit
  end

  return result
end

--------------------------------------------------------------------------------
-- SELECT
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:select(params, flag)
  return 'SELECT ' .. self.table_name .. '.* FROM ' .. self.table_name .. " " .. self:where(params, flag)
end

--------------------------------------------------------------------------------
-- DELETE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:destroy(record)
  local values = {[self.primary_key] = record[self.primary_key]}
  local sql    = "DELETE FROM " .. self.table_name .. " " .. self:where(values)
  local result = self:execute(sql)
  self.cache[record:cacheKey()] = false
  return result
end

-------------------------------------------------------------------------------
-- COUNT
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:count()
  error("count not implemented")
end

--------------------------------------------------------------------------------
-- INSERT
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:insert(values)
  error('not implemeted')
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:update(values)
  error('not implemeted')
end

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:create(record)
  error('not implemeted')
end

--------------------------------------------------------------------------------
-- FIND
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:find(params)

  if params[self.primary_key] then
    local key = self.table_name .. '_' .. tostring(params[self.primary_key])
    if ActiveRecord_Adapter.cache[key] then
      return ActiveRecord_Adapter.cache[key]
    end
  end

  local sql  = self:select(params, true)
  return self:fetch(sql)
end

--------------------------------------------------------------------------------
-- ALL
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:all(params)
  local sql    = self:select(params)
  local cursor = self:execute(sql)
  local result = Array.new()
  for row in cursor:each() do
    table.insert(result, self:parser_fetch(row))
  end
  cursor:close()
  return result
end

--------------------------------------------------------------------------------
-- COLUMNS
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:columns(sql)
  error('not implemeted')
end


--------------------------------------------------------------------------------
-- LAST_ID
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:lastid()
  error('not implemeted')
end

--------------------------------------------------------------------------------
-- EXECUTE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:execute(sql)
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

function ActiveRecord_Adapter:query(sql)
  local time   = os.microtime()
  local cursor = self:execute(sql)
  time = os.microtime() - time
  if ActiveRecord.debug then
    --print(sql .. string.format(" (%.3f) secs", time))
  end
  ActiveRecord.time = ActiveRecord.time + time
  return cursor
end

--------------------------------------------------------------------------------
-- FETCH
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:fetch(sql)
  local cursor = self:execute(sql)
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

function ActiveRecord_Adapter:parser_fetch(res)
  local key  = self.table_name .. '_' .. tostring(res[self.primary_key])

  if ActiveRecord_Adapter.cache[key] then
    return ActiveRecord_Adapter.cache[key]
  else
    local neat = {}

    res.new_record = false
    for column, properties in pairs(self:columns()) do
      res[column]  = self:parser_value(properties.format, res[column])
      neat[column] = res[column]
    end

    ActiveRecord_Adapter.neat[key]  = neat
    ActiveRecord_Adapter.cache[key] = res

    return self.record_class.new(res)
  end
end

--------------------------------------------------------------------------------
-- BEGIN
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:begin()
  return self:execute("BEGIN")
end

--------------------------------------------------------------------------------
-- ROLLBACK
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:rollback()
  ActiveRecord_Adapter.errors = {}
  ActiveRecord_Adapter.cache  = {}
  return self:execute("ROLLBACK")
end

--------------------------------------------------------------------------------
-- COMMIT
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:commit()
  ActiveRecord_Adapter.errors = {}
  ActiveRecord_Adapter.cache  = {}
  return self:execute("COMMIT")
end

--------------------------------------------------------------------------------
-- POPULATE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:populate(record, params)
  for column, properties in pairs(self:columns()) do
    if params[column] then
      record[column] = self:parser_value(properties.format, params[column])
    end
  end
end

--------------------------------------------------------------------------------
-- BANG !
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:bang(record)
  local flag = false
  for k, v in pairs(record.errors) do
    flag = true
    break
  end
  if flag then
    local errors = record.errors
    record.errors = {}
    errors.traceback = debug.traceback()
    error(errors)
  end
end

--------------------------------------------------------------------------------
-- VALIDATE
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:validate(record, params)
  self["validate" .. params[1]:capitalize()](self, record, params)
end

function ActiveRecord_Adapter:validatePresence(record, params)
  if record[params.column] == nil or record[params.column] == '' then
    record.errors[params.column] = params.message
  end
end

function ActiveRecord_Adapter:validateBoolean(record, params)
  if not parse.isBoolean(record[params.column]) then
    record.errors[params.column] = params.message
  end
end

function ActiveRecord_Adapter:validateLength(record, params)
  if type(record[params.column]) == 'string' then
    local length = #record[params.column]
    if params.min and length < params.min then
      record.errors[params.column] = params.message
    end
    if params.max and length > params.max then
      record.errors[params.column] = params.message
    end
  else
    record.errors[params.column] = params.message
  end
end

function ActiveRecord_Adapter:validateUnique(record, params)
  local value  = record[params.column]
  if value ~= nil and value ~= '' then
    local result = self:all{ [params.column] = value }
    if record[self.primary_key] == nil and #result > 0 then
      record.errors[params.column] = params.message
    else
      for _, res in ipairs(result) do
        if record[self.primary_key] ~= res[self.primary_key] then
          record.errors[params.column] = params.message
          break
        end
      end
    end
  end
end

-------------------------------------------------------------------------------
-- LOAD
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:sql(name, params)
  local table   = self.record_class.table_name
  local query   = (self.record_class.query_prefix or '') .. 'query/' .. table
    query  = query .. '/' .. name .. '.sql'
  local values  = self.record_class.where(params)
  if values == nil then
    error(self.record_class.className .. "#where not return valid query")
  end
  local binding = values.binding
  if not os.exists(query) then
    error(query .. ' file not exists')
  end
  local sql     = os.read(query)
  local where   = self.record_class.adapter():where(values)

  if binding then
    for index, value in pairs(binding) do
      sql = string.replaceAll(sql, '$' .. index, format[type(value)](value))
    end
  end

  return (sql .. where)
end

-------------------------------------------------------------------------------
-- CREATE TIMESTAMP
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:createTimestamp()
  if CHARON_ENV == 'test' then
    local clock  = tostring(os.clock())
    local index  = clock:indexOf('.')
    return os.date("%Y/%m/%d %H:%M:%S.") .. clock:mid(index+1, -1)
  else
    return os.date("%Y/%m/%d %H:%M:%S")
  end
end

-------------------------------------------------------------------------------
-- TABLE EXISTS
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:tableExists(tableName)
  error("tableExists not implemented")
end

-------------------------------------------------------------------------------
-- PREPARE MIGRATIONS
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:prepareMigration()
  error("prepare migrations not implemented")
end

--------------------------------------------------------------------------------
-- SET
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:set(record, column, value)
  local properties = self:columns()[column]
  record[column] = self:parser_value(properties.format, value)
end

--------------------------------------------------------------------------------
-- READ
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:read(record, column)
  local value = record[column]
  column = self:columns()[column]
  if value == nil or column == nil then
    return nil
  else
    return self:read_value(column.format, value)
  end
end

function ActiveRecord_Adapter:read_value(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self['read_value_' .. format](value)
  end
end

function ActiveRecord_Adapter.read_value_string(value)
  return value
end

function ActiveRecord_Adapter.read_value_time(value)
  return value
end

function ActiveRecord_Adapter.read_value_timestamp(value)
  return value:toDateTime()
end

function ActiveRecord_Adapter.read_value_date(value)
  return value
end

function ActiveRecord_Adapter.read_value_number(value)
  if tostring(value):contains(',') then
    return tonumber(value:replaceChars('.', ''):replaceChars(',', '.'))
  else
    return tonumber(value)
  end
end

function ActiveRecord_Adapter.read_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_Adapter.read_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_Adapter.read_value_table(value)
  return table.concat(value, ',')
end


--------------------------------------------------------------------------------
-- GET
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:get(record, column, default)
  local value = record[column]
  column = self:columns()[column]
  if value == nil or column == nil then
    return default
  else
    return self:read_value(column.format, value) or default
  end
end

--------------------------------------------------------------------------------
-- COUNT
--------------------------------------------------------------------------------

function ActiveRecord_Adapter:count(params)
  local sql    = "SELECT COUNT(*) count_all FROM " .. self.table_name .. " " .. self:where(params)
  local cursor = self:execute(sql)
  local res    = cursor:fetch({}, 'a')
  cursor:close()
  return tonumber(res.count_all)
end

-------------------------------------------------------------------------------
-- CHANGES
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:changes(record)
  local changes = {}
  local key     = record:cacheKey()
  local neat    = ActiveRecord_Adapter.neat[key] or {}

  for column, properties in pairs(self:columns()) do
    if record[column] ~= neat[column] then
      changes[column] = { neat[column], record[column] }
    end
  end

  return changes
end

-------------------------------------------------------------------------------
-- PARSER VALUE
-------------------------------------------------------------------------------

function ActiveRecord_Adapter:parser_value(format, value)
  if format == nil or value == nil then
    return nil
  else
    return self['parser_value_' .. format](value)
  end
end

function ActiveRecord_Adapter.parser_value_string(value)
  return value
end

function ActiveRecord_Adapter.parser_value_time(value)
  if value == '' then
    return nil
  else
    return value
  end
end

function ActiveRecord_Adapter.parser_value_date(value)
  if value == '' then
    return nil
  else
    return value
  end
end

function ActiveRecord_Adapter.parser_value_number(value)
  value = tostring(value)
  if value:contains(',') then
    return tonumber(value:replaceChars('.', ''):replaceChars(',', '.'))
  else
    return tonumber(value)
  end
end

function ActiveRecord_Adapter.parser_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_Adapter.parser_value_timestamp(value)
  if value == '' then
    return nil
  else
    return value
  end
end

return ActiveRecord_Adapter
