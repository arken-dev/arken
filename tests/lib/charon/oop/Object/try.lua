local Object = require('arken.oop.Object')
local Class  = require('arken.oop.Class')
local test   = {}

test.should_return_object_in_many_calls = function()
  local obj = Object.new()
  assert( obj:try('test1'):try('test2'):try('test3'):try('test4').test5 == nil )
end

test.should_return_object_in_many_calls_by_class = function()
  local MyClass = Class.new("MyClass")
  local obj = MyClass.new()
  assert( obj:try('test1'):try('test2'):try('test3'):try('test4').test5 == nil )
end

test.should_return_value_in_many_calls_by_class = function()
  local MyClass = Class.new("MyClass")
  function MyClass:cancel()
    return 'canceled'
  end
  local obj = MyClass.new()
  assert( obj:try('cancel') == 'canceled' )
end

return test
