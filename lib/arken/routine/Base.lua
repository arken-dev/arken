local routine = require('arken.routine')
local Class = require("arken.oop.Class")
local Base  = Class.new("arken.routine.Base")

Base.inherit = function(class)
  function class.new(params)
    local params = params or routine.parseArgs(arg)
    local obj = { __params = params }
    setmetatable(obj, class)
    obj:initialize()
    return obj
  end
end

function Base:params(rebuild)
  if self.__params == nil or rebuild then
    local routine = require('arken.routine')
    self.__params = routine.parseArgs(arg)
  end
  return self.__params
end

return Base
