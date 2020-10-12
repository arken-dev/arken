local Class = require('arken.oop.Class')
local test  = {}

test.before = function()
  Class.classes = {}
end

test.should_create_new_class_and_register = function()
  local MyClass = Class.new("MyClass")
  assert( MyClass == Class.lookup("MyClass") )
end

test.should_define_className = function()
  local MyClass = Class.new("MyClass")
  assert( MyClass.className == "MyClass" )
end

test.should_define_class_with_inheritancy = function()
  local MyClass = Class.new("MyClass")
  local OtherClass = Class.new("OtherClass", "MyClass")
  assert( OtherClass.superClass == MyClass )
end

test.should_redefine_class_with_inheritancy = function()
  local MyClass = Class.new("MyClass")
  local OtherClass = Class.new("OtherClass", "MyClass")
  assert( OtherClass.superClass == MyClass )
  local OtherClass = Class.new("OtherClass", "MyClass")
  assert( OtherClass.superClass == MyClass )
end

test.should_execute_inherit_method = function()
  local MyClass = Class.new("MyClass")
  MyClass.inherit = function(class)
    class.heyhere = true
  end
  local OtherClass = Class.new("OtherClass", "MyClass")
  assert( OtherClass.heyhere == true )
end

test.should_redefine_class_with_execute_inherit_method = function()
  local MyClass = Class.new("MyClass")
  MyClass.inherit = function(class)
    class.heyhere = true
  end
  local OtherClass1 = Class.new("OtherClass", "MyClass")
  assert( OtherClass1.heyhere == true )

  MyClass.inherit = function(class)
    class.heyhere = "hey !!!"
  end

  local OtherClass2 = Class.new("OtherClass", "MyClass")
  assert( OtherClass2.heyhere == 'hey !!!' )
  assert( OtherClass1 == OtherClass2 )
end

test.should_execute_initialize_instances = function()
  local MyClass = Class.new("MyClass")
  function MyClass:initialize()
    self.instance_value = 'hey here !!!'
  end
  local obj = MyClass.new()
  assert( obj.instance_value == 'hey here !!!' )
  assert( obj.class == MyClass )
end

return test
