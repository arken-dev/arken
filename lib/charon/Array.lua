local Class = require('charon.oop.Class')
local Array = Class.new("Array")

function Array:each()
  local i = 0
  local n = table.getn(self)
  return function()
    i = i + 1
    if i <= n then return self[i] end
  end
end

function Array:at(i)
  return self[i]
end

function Array:size()
  return #self
end

return Array
