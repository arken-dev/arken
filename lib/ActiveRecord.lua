require 'ActiveRecord.Adapter'

ActiveRecord = Class.new("ActiveRecord")

ActiveRecord.time = 0

-------------------------------------------------------------------------------
-- ActiveRecord#inherit
-------------------------------------------------------------------------------

ActiveRecord.inherit = function(class)

  -----------------------------------------------------------------------------
  -- DEFAULT PROPERTIES
  -----------------------------------------------------------------------------

  class.primary_key      = 'id'
  class.table_name       = class.class_name:underscore()
  class.adapter_instance = class.class_name .. 'AdapterInstance'

  ------------------------------------------------------------------------------
  -- RELACIONAMENTOS
  ------------------------------------------------------------------------------

  class.belongs = function(params)
    class[params.name] = function(self)
      if self[params.foreign_key] == nil then
        return nil
      else
        local  record = require(params.record)
        return record.find{[record.primary_key] = self[params.foreign_key]}
      end
    end
  end

  class.hasOne = function(params)
    class[params.name] = function(self)
      if self[self.primary_key] == nil then
        return nil
      else
        local  record = require(params.record)
        return record.find{[params.foreign_key] = self[self.primary_key]}
      end
    end
  end

  class.hasMany = function(params)
    class[params.name] = function(self)
      local record     = require(params.record)
      local conditions = params.conditions or {}
      conditions[params.foreign_key] = self[record.primary_key]
      if params.order then
        conditions.order = params.order
      end
      return record.all(conditions)
    end
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#find
  -----------------------------------------------------------------------------

  class.find = function(params)
    return class.adapter():find(params)
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#exists
  -----------------------------------------------------------------------------

  class.exists = function(params)
    return class.adapter():find(params) ~= nil
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#all
  -----------------------------------------------------------------------------

  class.all = function(params)
    return class.adapter():all(params or {})
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#adapter()
  -----------------------------------------------------------------------------

  class.adapter = function()
    local adapter_instance = class.adapter_instance
    if class[adapter_instance] == nil then
      class[adapter_instance] = class.loadAdapter()
    end
    return class[adapter_instance]
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#loadAdapter()
  -----------------------------------------------------------------------------

  class.loadAdapter = function(adapter_name)
    local config  = class.loadConfig()
    local adapter = adapter_name or config.adapter
    return require(adapter).new{
      record_class = class,
      table_name   = class.table_name,
      primary_key  = class.primary_key,
      user         = config.user,
      password     = config.password,
      database     = config.database,
      host         = config.host
    }
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#loadConfig()
  -----------------------------------------------------------------------------

  class.loadConfig = function()
    local JSON   = require 'JSON'
    local raw    = os.read("config/active_record.json")
    local env    = OBERON_ENV or 'development'
    local config = JSON:decode(raw)
    if type(config) == 'table' then
      return config[env]
    else
      error "config/active_record.json invalid"
    end
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#begin
  ------------------------------------------------------------------------------

  function class.begin()
    class.adapter():begin()
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#rollback
  ------------------------------------------------------------------------------

  function class.rollback()
    class.adapter():rollback()
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#commit
  ------------------------------------------------------------------------------

  function class.commit()
    class.adapter():commit()
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#clear
  ------------------------------------------------------------------------------
  -- clear all cache
  function class.clear()
    ActiveRecord_Adapter._cache = {}
    ActiveRecord_Adapter.cache = {}
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#where
  ------------------------------------------------------------------------------
  function class.where(params)
    error('where not implemented in ' .. class.class_name)
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#columns
  ------------------------------------------------------------------------------

  function class.columns()
    return class.adapter():columns()
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#query
  ------------------------------------------------------------------------------
  function class.query(name, params)
    local sql = class.adapter():sql(name, params)
    return class.adapter():query(sql)
  end

end

-------------------------------------------------------------------------------
-- ActiveRecord#initialize
-------------------------------------------------------------------------------

function ActiveRecord:initialize()
  self.errors = {}
  if self.new_record == nil then
    self.adapter():defaultValues(self)
  end
end

-------------------------------------------------------------------------------
-- ActiveRecord#create
-------------------------------------------------------------------------------

function ActiveRecord:create()
  return self.adapter():create(self)
end

-------------------------------------------------------------------------------
-- ActiveRecord#update
-------------------------------------------------------------------------------

function ActiveRecord:update()
  return self.adapter():update(self)
end

-------------------------------------------------------------------------------
-- ActiveRecord#save
-------------------------------------------------------------------------------

function ActiveRecord:save()
  if self.new_record then
    self:create()
  else
    self:update()
  end
  return self
end

-------------------------------------------------------------------------------
-- ActiveRecord#destroy
-------------------------------------------------------------------------------

function ActiveRecord:destroy()
  return self.adapter():destroy(self)
end

-------------------------------------------------------------------------------
-- ActiveRecord#pcall
-------------------------------------------------------------------------------

function ActiveRecord:pcall(method, params)
  return pcall(self[method], self, params)
end

-------------------------------------------------------------------------------
-- ActiveRecord#cacheKey()
-------------------------------------------------------------------------------

function ActiveRecord:cacheKey()
  return self.table_name .. '_' .. tostring(self[self.primary_key])
end

-------------------------------------------------------------------------------
-- ActiveRecord#read()
-------------------------------------------------------------------------------

function ActiveRecord:read(column)
  return self.adapter():read(self, column)
end

-------------------------------------------------------------------------------
-- ActiveRecord#get()
-------------------------------------------------------------------------------

function ActiveRecord:get(column, default)
  return self.adapter():get(self, column, default)
end


-------------------------------------------------------------------------------
-- ActiveRecord#populate()
-------------------------------------------------------------------------------

function ActiveRecord:populate(params)
  return self.adapter():populate(self, params)
end

-------------------------------------------------------------------------------
-- ActiveRecord#validate()
-------------------------------------------------------------------------------

function ActiveRecord:validate(params)
  return self.adapter():validate(self, params)
end

-------------------------------------------------------------------------------
-- ActiveRecord#changes()
-------------------------------------------------------------------------------

function ActiveRecord:changes()
  return self.adapter():changes(self)
end

-------------------------------------------------------------------------------
-- ActiveRecord#dup()
-------------------------------------------------------------------------------

function ActiveRecord:dup()
  local dup = {}
  for k, v in pairs(self) do
    dup[k] = v
  end
  return dup
end


return ActiveRecord
