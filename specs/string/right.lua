local should = require "test.should"
local test   = {}

test['should end of string'] = function()
  str1 = "this is a test"
  str2 = "test"
  should.be_equal( str1:right(4), str2 )
end

test['should before string with new line'] = function()
  str1 = "this is a test\n"
  str2 = "test\n"
  should.be_equal( str1:right(5), str2 )
end

return test
