local Class = require('charon.oop.Class')
local Array = Class.new("Array")

function Array:at(i)
  return self[i]
end

function Array:size()
  return #self
end

return Array
