local Class = require("arken.oop.Class")
local Base  = Class.new("arken.routine.Base")

Base.inherit = function(class)
  class.help = {}
  function class.new(params)
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
