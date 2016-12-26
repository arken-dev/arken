local Class = require('charon.oop.Class')
local test  = {}

test.should_execute_prepare = function()
  local MyClass = Class.new("MyClass")
  function MyClass:cancel()
    self.canceled = true
  end
  function MyClass:cancelPrepare()
    self.cancelprepare = 'hey here !!!'
  end
  MyClass.contract('cancel')
  local obj = MyClass.new()
  assert(obj.cancelprepare == nil)
  assert(obj.canceled == nil)
  obj:cancel()
  assert(obj.cancelprepare == 'hey here !!!', obj.cancelprepare)
  assert(obj.canceled == true)
end

return test
