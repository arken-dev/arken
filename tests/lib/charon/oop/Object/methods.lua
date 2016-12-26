local Class = require('charon.oop.Class')
local test  = {}

test.should_list_of_methods = function()
  local MyClass = Class.new("MyClass")
  function MyClass:cancel()
  end
  local obj = MyClass.new()
  assert(obj.class == MyClass)
  assert( obj:methods()[1] == 'cancel', obj:methods()[1] )
end

return test
