local Class    = require 'charon.oop.Class'
local contract = require 'charon.contract'
local test     = {}

test.before = function()
  Class.classes = {}
end

test.should_create_prepare_function = function()
  local MyClass = Class.new("MyClass")
  MyClass.cancel = function()
  end
  contract.create(MyClass, 'cancel')
  assert(type(MyClass.cancelPrepare) == 'function')
end

test.should_create_validate_function = function()
  local MyClass = Class.new("MyClass")
  MyClass.cancel = function()
  end
  contract.create(MyClass, 'cancel')
  assert(type(MyClass.cancelValidate) == 'function')
end

test.should_create_before_function = function()
  local MyClass = Class.new("MyClass")
  MyClass.cancel = function()
  end
  contract.create(MyClass, 'cancel')
  assert(type(MyClass.cancelBefore) == 'function')
end

test.should_create_before_function = function()
  local MyClass = Class.new("MyClass")
  MyClass.cancel = function()
  end
  contract.create(MyClass, 'cancel')
  assert(type(MyClass.cancelAfter) == 'function')
end

test.should_execute_methods_and_sequencie = function()
  local MyClass = Class.new("MyClass")

  function MyClass:cancel(params)
    params.total  = params.total + 1
    params.cancel = 'cancel !!! ' .. params.total
  end

  function MyClass:cancelPrepare(params)
    params.total  = params.total + 1
    params.prepare = 'prepare !!! ' .. params.total
  end

  function MyClass:cancelValidate(params)
    params.total  = params.total + 1
    params.validate = 'validate !!! ' .. params.total
  end

  function MyClass:cancelBefore(params)
    params.total  = params.total + 1
    params.before = 'before !!! ' .. params.total
  end

  function MyClass:cancelAfter(params)
    params.total  = params.total + 1
    params.after = 'after !!! ' .. params.total
  end

  MyClass.contract('cancel')

  local params = { total = 0 }
  local myobj  = MyClass.new()

  myobj:cancel(params)

  assert(params.prepare == 'prepare !!! 1', params.prepare)
  assert(params.validate == 'validate !!! 2', params.validate)
  assert(params.before == 'before !!! 3', params.before)
  assert(params.cancel == 'cancel !!! 4', params.cancel)
  assert(params.after == 'after !!! 5', params.after)
end

return test
