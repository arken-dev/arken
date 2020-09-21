local Class = require('arken.oop.Class')
local test  = {}

test.should_execute_prepare = function()
  local MyClass = Class.new("MyClass")
  assert(type(MyClass.myMethod) == 'nil')
  MyClass.mixin('util.oop.MyMethods')
  assert(type(MyClass.myMethod) == 'function')
end

test.should_remove_mixin_methods_in_redefine_class = function()
  local MyClass = Class.new("MyClass")
  assert(type(MyClass.myMethod) == 'nil')
  MyClass.mixin('util.oop.MyMethods')
  assert(type(MyClass.myMethod) == 'function')
  MyClass = Class.new("MyClass")
  assert(type(MyClass.myMethod) == 'nil')
end


return test
