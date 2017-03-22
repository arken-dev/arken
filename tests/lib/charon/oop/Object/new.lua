local Object = require('charon.oop.Object')
local Class  = require('charon.oop.Class')
local test   = {}

test.should_execute_initialize_instances = function()
  local tbl = {}
  function tbl:initialize()
    self.instance_value = 'hey here !!!'
  end
  obj = Object.new(tbl)
  assert( obj.instance_value == 'hey here !!!' )
  assert( obj == tbl )
end

test.should_execute_object_return_nil = function()
  assert( Object:initialize() == nil )
end

return test
