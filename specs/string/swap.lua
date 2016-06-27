local should = require "test.should"
local test   = {}

test['should swap a smaller string for greater string'] = function()
  local str = "this is a test"
  should.be_equal( str:swap(" is ", " is not "), "this is not a test" )
end

test['should swap a greater string for smaller string'] = function()
  local str = "this is not a test"
  should.be_equal( str:swap(" is not ", " is "), "this is a test" )
end

test['should swap a string for empty string'] = function()
  local str = "this is not a test"
  should.be_equal( str:swap(" not ", " "), "this is a test" )
end

test['should preserve string not valid string'] = function()
  local str = "this is not a test"
  should.be_equal( str:swap("yes", ""), "this is not a test" )
end

return test
