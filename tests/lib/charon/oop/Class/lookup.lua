local Class = require 'charon.oop.Class'
local test  = {}

test.should_return_stored = function()
  local MyClass = {}
  Class.classes['MyClass'] = MyClass
  assert( Class.lookup('MyClass') == MyClass )
end

return test
