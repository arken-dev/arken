local Class = require('arken.oop.Class')
local test  = {}

test.should_execute_method_create_by_inherit_method = function()
  local MyClass = Class.new("MyClass")
  MyClass.inherit = function(class)
    class.heyhere = function()
      return 'heyhere'
    end
  end
  local OtherClass = Class.new("OtherClass", "MyClass")
  local obj = OtherClass.new()
  assert( obj:heyhere() == 'heyhere' )
end

return test
