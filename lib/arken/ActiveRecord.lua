-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local json    = require("arken.json")
local Class   = require("arken.oop.Class")
local Array   = require("arken.Array")
ActiveRecord  = Class.new("ActiveRecord")

ActiveRecord.time     = 0
ActiveRecord.schema   = false
ActiveRecord.fixtures = false

-------------------------------------------------------------------------------
-- ActiveRecord#inherit
-------------------------------------------------------------------------------

ActiveRecord.inherit = function(class)

  -----------------------------------------------------------------------------
  -- DEFAULT PROPERTIES
  -----------------------------------------------------------------------------

  class.primaryKey     = 'id'
  class.tableName       = class.className:underscore()
  class.adapterInstance = class.className .. 'AdapterInstance'

  ------------------------------------------------------------------------------
  -- RELACIONAMENTOS
  ------------------------------------------------------------------------------

  class.belongsTo = function(params)
    class[params.name] = function(self)
      if self[params.foreignKey] == nil then
        return nil
      else
        local  record = Class.lookup(params.record)
        return record.find{[record.primaryKey] = self[params.foreignKey]}
      end
    end
  end

  class.hasOne = function(params)
    class[params.name] = function(self)
      if self[class.primaryKey] == nil then
        return nil
      else
        local  record = Class.lookup(params.record)
        return record.find{[params.foreignKey] = self[self.primaryKey]}
      end
    end
  end

  class.hasMany = function(params)
    class[params.name] = function(self)
      if self[class.primaryKey] == nil then
        return nil
      else
        local record     = Class.lookup(params.record)
        local conditions = params.conditions or {}
        conditions[params.foreignKey] = self[record.primaryKey]
        if params.order then
          conditions.order = params.order
        end
        return record.all(conditions)
      end
    end
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#find
  -----------------------------------------------------------------------------

  class.find = function(params)
    params = params or {}
    params.limit = 1
    return class.adapter():find(params)
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#last
  -----------------------------------------------------------------------------

  class.last = function(params)
    params = params or {}
    params.limit = 1
    params.order = string.format("%s.%s DESC", class.tableName, class.primaryKey)
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
  -- ActiveRecord#count
  -----------------------------------------------------------------------------

  class.count = function(params)
    return class.adapter():count(params or {})
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#reset()
  -----------------------------------------------------------------------------

  class.reset = function()
    class[class.adapterInstance] = nil
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#adapter()
  -----------------------------------------------------------------------------

  class.adapter = function(force)
    local adapterInstance = class.adapterInstance
    if class[adapterInstance] == nil or force then
      class[adapterInstance] = class.factoryAdapter()
    end
    return class[adapterInstance]
  end

  -----------------------------------------------------------------------------
  -- ActiveRecord#factoryAdapter()
  -----------------------------------------------------------------------------

  class.factoryAdapter = function(params)
    local params  = params or {}
    local config  = class.loadConfig()
    local adapter = params.adapter or config.adapter
    return Class.lookup(adapter).new{
      record_class = class,
      tableName    = params.tableName  or class.tableName,
      primaryKey   = params.primaryKey or class.primaryKey,
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
    local template = require 'arken.template'
    local config   = "config/active_record.json"

    if not os.exists(config) then
      error("file " .. config .. " not exists")
    end

    local raw  = template.execute(config)
    local env  = ARKEN_ENV or 'development'
    local data = json.decode(raw)
    if type(data) == 'table' then
      return data[env]
    else
      error(config .. " invalid")
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
    local Adapter = require("arken.ActiveRecord.Adapter")
    Adapter.neat  = {}
    Adapter.cache = {}
    for _, cursor in ipairs(Adapter.cursor) do
      cursor:close()
    end
    Adapter.cursor = {}
  end

  ------------------------------------------------------------------------------
  -- ActiveRecord#where
  ------------------------------------------------------------------------------
  function class.where(params)
    error('where not implemented in ' .. class.className)
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

  -------------------------------------------------------------------------------
  -- ActiveRecord#create
  -------------------------------------------------------------------------------

  function class.create(record)
    if record.className == nil then
      record = class.new(record)
    end
    return class.adapter():create(record)
  end

end

-------------------------------------------------------------------------------
-- ActiveRecord#initialize
-------------------------------------------------------------------------------

function ActiveRecord:initialize()
  self.errors = Array.new()
  if self.newRecord == nil then
    self.adapter():defaultValues(self)
  end
end

-------------------------------------------------------------------------------
-- ActiveRecord#update
-------------------------------------------------------------------------------

function ActiveRecord:update()
  if self.destroy_record then
    error("record destroyed")
  end
  return self.adapter():update(self)
end

-------------------------------------------------------------------------------
-- ActiveRecord#save
-------------------------------------------------------------------------------

function ActiveRecord:save()
  if self.newRecord then
    self:create({})
  else
    self:update({})
  end
  return self
end

-------------------------------------------------------------------------------
-- ActiveRecord#destroy
-------------------------------------------------------------------------------

function ActiveRecord:destroy()
  self.destroy_record = true
  return self.adapter():destroy(self)
end

-------------------------------------------------------------------------------
-- ActiveRecord#cacheKey()
-------------------------------------------------------------------------------

function ActiveRecord:cacheKey()
  return self.tableName .. '_' .. tostring(self[self.primaryKey])
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
-- ActiveRecord#set()
-------------------------------------------------------------------------------

function ActiveRecord:set(column, value)
  return self.adapter():set(self, column, value)
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
-- WAS
-------------------------------------------------------------------------------

function ActiveRecord:was()
  return self.adapter():was(self)
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
