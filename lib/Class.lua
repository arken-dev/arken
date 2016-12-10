-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

require 'Object'

Class = Class or {}
Class.classes    = {}
Class.class_name = "Class"

function Class.new(className, inheritedBy, params)
  local class;
  if inheritedBy then
    inheritedBy = require(inheritedBy, true)
  end
  -- recreate empty class
  if Class.classes[className] then
    class = Class.classes[className]
    if inheritedBy then
      setmetatable(Class.classes[className], inheritedBy)
    end
    for key, value in pairs(class) do
      class[key] = nil
    end

    -- name class
    class.class_name = className
    class.__index    = class
    class.class      = class

    if inheritedBy then
      class.superclass = inheritedBy
      if class.inherit then
        class.inherit(class, params)
      end
    else
      class.superclass = Class
    end
  else
    Class.classes[className] = {}
    class = Class.classes[className]
    -- name class
    class.class_name = className --'class'
    class.__index    = class
    class.class      = class

    if inheritedBy then
      setmetatable(Class.classes[className], inheritedBy)
      if class.inherit then
        class.inherit(class, params)
      end
      class.superclass = inheritedBy
    else
      setmetatable(Class.classes[className], Object)
      class.superclass = Class
    end
  end

  -- create new
  function class.new(record)
    local obj = record or {}
    setmetatable(obj, class)
    obj:initialize()
    return obj
  end

  -- contract
  function class.contract(name)
    local contract = require('contract')
    contract.create(class, name)
  end

  function class.mixin(path)
    package.mixed[path] = class.class_name
    local M = require(path, true)
    for key, value in pairs(M) do
      class[key] = value
    end
  end

  return class
end

return Class
