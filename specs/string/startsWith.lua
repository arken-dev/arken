local should = require "test.should"
local test   = {}

test['should true for compatible start string'] = function()
  str = "this is a test"
  should.be_valid( str:startsWith("this is") )
end

test['should false for incompatible start string'] = function()
  str = "this is a test"
  should.not_valid( str:startsWith("other is") )
end

return test
