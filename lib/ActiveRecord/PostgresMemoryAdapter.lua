require 'oberon.pgsql'

ActiveRecord_PostgresMemoryAdapter = Class.new(
  "ActiveRecord.PostgresMemoryAdapter",
  "ActiveRecord.PostgresAdapter"
)

ActiveRecord_PostgresMemoryAdapter._cache    = {}
ActiveRecord_PostgresMemoryAdapter.cache     = {}
ActiveRecord_PostgresMemoryAdapter.sequences = {}
ActiveRecord_PostgresMemoryAdapter.indexes   = {}

--------------------------------------------------------------------------------
-- CREATE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:create(record)
  record.id    = self:nextSequence()
  record.new_record = false
  self:getCache()[record:cacheKey()] = record
  self:getIndex()[record:cacheKey()] = record
  return record
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:update(record)
  if self.cache[self.table_name][record:cacheKey()] == false then
    error{"record is deleted"}
  end
  self.cache[self.table_name][record:cacheKey()] = record
end

--------------------------------------------------------------------------------
-- UPDATE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:delete(record)
  self.cache[self.table_name][record:cacheKey()] = false
end

--------------------------------------------------------------------------------
-- CLONE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:cloneByCache(key)
  local record = self:_getCache()[key]
  local tmp    = {}
  for k,v in pairs(record) do
    tmp[k] = v
  end
  self.cache[self.table_name][key] = self.record_class.new(tmp)
  return tmp
end

--------------------------------------------------------------------------------
-- RETRIVE FROM CACHE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:retrieveFromCache(params)
  local key = nil
  local result = {}
  local order  = params.order
  if order then
    params.order = nil
    if self:columns()[order] == nil then
      error('order ' .. order .. ' is not attribute in ' .. self.table_name)
    end
  end

  if params[self.primary_key] then
    key = self.table_name .. '_' .. tostring(params[self.primary_key])
    if self:getCache()[key] == self:_getCache()[key] then
      table.insert(result, self:cloneByCache(key))
    else
      table.insert(result, self:getCache()[key])
    end
    return result
  end

  for key, _ in pairs(self:getIndex()) do
    local record = self:getCache()[key]
    if record then -- record pode ter sido excluida
      local flag = true
      for k,v in pairs(params) do
        if record[k] ~= v then
          flag = false
          break
        end
      end

      if flag then
        local key = record:cacheKey()
        if self:getCache()[key] == self:_getCache()[key] then
          table.insert(result, self:cloneByCache(key))
        else
          table.insert(result, self:getCache()[key])
        end
      end
    end
  end

  if order then
    table.sort(result, function(rec1, rec2)
      return tostring(rec1[order]) < tostring(rec2[order])
    end)
  end

  return result
end

--------------------------------------------------------------------------------
-- FIND
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:find(params)
  return self:retrieveFromCache(params)[1]
end

--------------------------------------------------------------------------------
-- ALL
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:all(params)
  return self:retrieveFromCache(params)
end

--------------------------------------------------------------------------------
-- LOAD TABLE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:loadTable()
  if self.table_name == 'active_record' then
    self.indexes[self.table_name] = {}
    self.cache[self.table_name]   = {}
    self._cache[self.table_name]  = {}
  end

  if self.indexes[self.table_name] == nil then
    print('indexes: ' .. self.table_name)
    self.indexes[self.table_name] = {}

    local _cache = {}
    local  cache = {}

    self._cache[self.table_name] = _cache
    self.cache[self.table_name]  =  cache

    setmetatable(cache, _cache)
    _cache.__index = _cache

    local sql    = 'SELECT * FROM ' .. self.table_name
    local res    = self:execute(sql)
    print('load table ' .. self.table_name)
    print(sql)
    for row in res:rows() do
      self:parser_fetch(row)
    end
  end
end

--ActiveRecord_PostgresMemoryAdapter:contract('loadTable')

--------------------------------------------------------------------------------
-- LOAD SEQUENCE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:loadSequence()
  local seq   = self.table_name .. '_' .. self.primary_key .. '_seq'
  if self.table_name == 'active_record' then
    self.sequences[seq] = 1
  else
    local sql  = 'SELECT * FROM ' .. seq
    local res  = self:execute(sql)
    local data = res:fetch(sql)
    print('load sequence' .. self.table_name)
    print(sql)
    self.sequences[seq] = tonumber(data.last_value)
  end
end


--------------------------------------------------------------------------------
-- NEXT SEQUENCE
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:nextSequence()
  local seq   = self.table_name .. '_' .. self.primary_key .. '_seq'
  local value = self:getSequence()
  self.sequences[seq] = value + 1
  return self.sequences[seq]
end

--------------------------------------------------------------------------------
-- FETCH
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:fetch(sql)
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

function ActiveRecord_PostgresMemoryAdapter:parser_fetch(res)
  res.new_record = false
  -- disable convert value in driver
--[[
  for column, properties in pairs(self:columns()) do
    res[column] = self:parser_value(properties.format, res[column])
  end
  ]]
  local key = self.table_name .. '_' .. tostring(res[self.primary_key])
  self.indexes[self.table_name][key] = true
  self._cache[self.table_name][key] = self.record_class.new(res)
  return res
end

--------------------------------------------------------------------------------
-- BEGIN
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:begin()
  --self.cache = {}
end

--------------------------------------------------------------------------------
-- getSequence
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:getSequence()
  local seq = self.table_name .. '_' .. self.primary_key .. '_seq'
  if self.sequences[seq] == nil then
    self:loadSequence()
  end

  return self.sequences[seq]
end

--------------------------------------------------------------------------------
-- getCache
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:getCache()
  if self.cache[self.table_name] == nil then
    self:loadTable()
  end

  return self.cache[self.table_name]
end

--------------------------------------------------------------------------------
-- _getCache
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:_getCache()
  if self._cache[self.table_name] == nil then
    self:loadTable()
  end

  return self._cache[self.table_name]
end


--------------------------------------------------------------------------------
-- getIndex
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:getIndex()
  if self.indexes[self.table_name] == nil then
    self:loadTable()
  end

  return self.indexes[self.table_name]
end


--------------------------------------------------------------------------------
-- BEGIN
--------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:rollback()
  for table_name, records in pairs(self.cache) do
    for key, record in pairs(records) do
      if self._cache[table_name][key] then
        self.cache[table_name][key] = nil
      else
        self.cache[table_name][key] = nil
        self.indexes[table_name][key] = nil
      end
    end
  end
end

-------------------------------------------------------------------------------
-- INITIALIZE
-------------------------------------------------------------------------------

function ActiveRecord_PostgresMemoryAdapter:initialize()
  self:loadTable()
  self:loadSequence()
end

return ActiveRecord_PostgresMemoryAdapter
