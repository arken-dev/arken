local Class = require("arken.oop.Class")
local Base  = Class.new("arken.routine.Base")

function Base:params(rebuild)
  if self.__params == nil or rebuild then
    local routine = require('arken.routine')
    self.__params = routine.parseArgs(arg)
  end
  return self.__params
end

return Base
