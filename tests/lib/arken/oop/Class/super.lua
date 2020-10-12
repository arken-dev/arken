local Class = require("arken.oop.Class")
local test  = {}

test.should_class_superclass_method = function()
  local ClassA = Class.new("ClassA__super")
  local ClassB = Class.new("ClassB__super", ClassA)

  function ClassA:test()
    return self.description .. ' ClassA'
  end

  function ClassB:test()
    return self.description .. ' ClassB'
  end

  local obj = ClassB.new{ description = 'Hello' }

  assert( obj:test() == 'Hello ClassB', obj:test() )
  assert( obj:super('test') == 'Hello ClassA', obj:super('test') )
end

test.should_class_superclass_method_with_param = function()
  local ClassA = Class.new("ClassA__super_param")
  local ClassB = Class.new("ClassB__super_param", ClassA)

  function ClassA:test(value)
    return self.description .. ' ClassA ' .. value
  end

  function ClassB:test(value)
    return self.description .. ' ClassB ' .. value
  end

  local obj = ClassB.new{ description = 'Hello' }

  assert( obj:test('works !') == 'Hello ClassB works !', obj:test('works !') )
  assert( obj:super('test', 'works !') == 'Hello ClassA works !', obj:super('test', 'works !') )
end

return test
