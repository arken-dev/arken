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
    for key, value in pairs(class) do
      class[key] = nil
    end

    -- name class
    class.class_name = className
    class.__index    = class
    class.class      = class

    if inheritedBy then
      class.superclass = inheritedBy
      class.inherit(class)
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

  function class.mixin(file_or_module)
    local M;
    if(type(file_or_module) == 'string') then
      M = require(file_or_module, true)
    else
      M = file_or_module
    end
    for key, value in pairs(M) do
      class[key] = value
    end
  end

  return class
end

return Class
