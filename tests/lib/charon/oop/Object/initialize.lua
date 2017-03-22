local Object = require('charon.oop.Object')
local Class  = require('charon.oop.Class')
local test   = {}

test.should_execute_initialize_instances = function()
  local MyClass = Class.new("MyClass")
  function MyClass:initialize()
    self.instance_value = 'hey here !!!'
  end
  local obj = MyClass.new()
  assert( obj.instance_value == 'hey here !!!' )
  assert( obj.class == MyClass )
end

test.should_execute_object_return_nil = function()
  assert( Object:initialize() == nil )
end

return test
