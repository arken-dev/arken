local Class = require('arken.oop.Class')
local test  = {}

test['should execute prepare'] = function()
  local MyClass = Class.new("MyClass")
  assert(type(MyClass.myMethod) == 'nil')
  MyClass.include('util.oop.MyMethods')
  assert(type(MyClass.myMethod) == 'function')
end

test['should remove include methods in redefine class'] = function()
  local MyClass = Class.new("MyClass")
  assert(type(MyClass.myMethod) == 'nil')
  MyClass.include('util.oop.MyMethods')
  assert(type(MyClass.myMethod) == 'function')
  MyClass = Class.new("MyClass")
  assert(type(MyClass.myMethod) == 'nil')
end

return test
