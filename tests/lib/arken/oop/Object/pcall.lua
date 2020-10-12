local Class = require("arken.oop.Class")
local test  = {}

test.should_pcall_method = function()
  local ClassA = Class.new("ClassA__pcall")
  function ClassA:test()
    error('test error')
  end

  local object = ClassA.new()
  local status, message = object:pcall('test')

  assert( status == false )
  assert( message:contains('test error') == true )
end

return test
