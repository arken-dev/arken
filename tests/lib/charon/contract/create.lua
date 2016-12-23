local Class    = require 'charon.oop.Class'
local contract = require 'charon.contract'
local tests    = {}

tests.should_create_prepare_function = function()
  local MyClass = Class.new("MyClass")
  MyClass.cancel = function()
  end
  contract.create(MyClass, 'cancel')
  assert(type(MyClass.cancelPrepare) == 'function')
end

return tests
