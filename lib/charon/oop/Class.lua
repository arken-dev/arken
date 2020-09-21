-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Object = require 'arken.oop.Object'

Class = Class or {}
Class.classes    = {}
Class.className  = "Class"

Class.new = function(className, inheritedBy, params)
  local class

  if type(inheritedBy) == 'string' then
    inheritedBy = Class.lookup(inheritedBy)
  end

  if Class.classes[className] then
    class = Class.classes[className]
    if inheritedBy then
      setmetatable(Class.classes[className], inheritedBy)
      class.superClass = inheritedBy
    end
    for key, value in pairs(class) do
      class[key] = nil
    end
  else
    Class.classes[className] = {}
    class = Class.classes[className]
  end

  -- name class
  class.className = className --'class'
  class.__index    = class
  class.class      = class

  -- create new
  function class.new(record)
    local obj = record or {}
    setmetatable(obj, class)
    obj:initialize()
    return obj
  end

  -- inherith
  if inheritedBy then
    setmetatable(Class.classes[className], inheritedBy)
    if class.inherit then
      class.inherit(class, params)
    end
    class.superClass = inheritedBy
  else
    setmetatable(Class.classes[className], Object)
    class.superClass = Class
  end

  function class:__tostring()
    return string.format('%s Class', self.className)
  end

  -- contract
  function class.contract(name)
    --if class[name .. "Body"] then
    --  error(string.format('contract %q in %s is already created', name, class.className))
    --else
      local contract = require('arken.contract')
      contract.create(class, name)
    --end
  end

  function class.mixin(path)
    package.mixed[path] = class.className
    local M = require(path)
    for key, value in pairs(M) do
      class[key] = value
    end
  end

  function class:super(method, ...)
    return self.superClass[method](self, ...)
  end

  return class
end

Class.lookup = function(name)
  return Class.classes[name] or Class.retrieve(name)
end

Class.retrieve = function(name)
  local flag, class_or_error = pcall(require, name)
  if flag then
    return class_or_error or flag
  end
  local flag, class = pcall(require, 'arken.' .. name)
  if flag then
    return class or flag
  else
    error(class_or_error)
  end
end

return Class
