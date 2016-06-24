require 'oberon.pgsql'

ActiveRecord_PostgresAdapter = Class.new("ActiveRecord_PostgresAdapter", "ActiveRecord.Adapter")

------------------------------------------------------------------------------
-- CACHE
-------------------------------------------------------------------------------

ActiveRecord_PostgresAdapter.cache = {}

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
  print(string.format(sql .. " (%.4f) secs", time))
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
  print(string.format(sql .. " (%.4f) secs", time))
  ActiveRecord.time = ActiveRecord.time + time
  return result
end

--------------------------------------------------------------------------------
-- INSERT
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:insert(record)
  self:bang(record)
  local sql = 'INSERT INTO'
  local col = ''
  local val = ''
  if self:columns().created_at then
    record.created_at = QDateTime.currentDateTime():toString()
  end
  if self:columns().updated_at then
    record.updated_at = record.created_at
  end
  for column, value in pairs(record) do
    if not self:isReserved(column) then
    --for column, properties in pairs(self:columns(table)) do
      if column ~= self.primary_key then
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

  return 'INSERT INTO ' .. self.table_name .. ' (' .. col .. ') VALUES (' .. val .. ') RETURNING id'
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
    record.updated_at = QDateTime.currentDateTime():toString()
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
  return result
end

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:create(record)
  local sql    = self:insert(record)
  local cursor = self:execute(sql)
  local row    = cursor:fetch({}, 'a')
  record.id    = tonumber(row[self.primary_key])
  record.new_record = false
  self.cache[record:cacheKey()] = record
  return record
end

--------------------------------------------------------------------------------
-- FIND
--------------------------------------------------------------------------------

function ActiveRecord_PostgresAdapter:find(params)

  if params[self.primary_key] then
    local key = self.table_name .. tostring(params[self.primary_key])
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
    local key = self.table_name .. tostring(data[self.primary_key])
    self.cache[key] = data
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
  return value
end

function ActiveRecord_PostgresAdapter.parser_time(value)
  return value
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
  local res    = result:fetch()
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
  res.new_record = false
  for column, properties in pairs(self:columns()) do
    res[column] = self:parser_value(properties.format, res[column])
  end
  return self.record_class.new(res)
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
  return value
end

function ActiveRecord_PostgresAdapter.parser_value_date(value)
  return value
end

function ActiveRecord_PostgresAdapter.parser_value_number(value)
  return tonumber(value)
end

function ActiveRecord_PostgresAdapter.parser_value_boolean(value)
  return toboolean(value)
end

function ActiveRecord_PostgresAdapter.parser_value_timestamp(value)
  return value
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
  return self:execute("ROLLBACK")
end

--------------------------------------------------------------------------------
-- COMMIT
--------------------------------------------------------------------------------
function ActiveRecord_PostgresAdapter:commit()
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

return ActiveRecord_PostgresAdapter
