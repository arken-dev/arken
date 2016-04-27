local should = require "test.should"
local test   = {}

test['should before of string'] = function()
  str1 = "this is a test"
  str2 = "this is"
  should.be_equal( str1:mid(0, 7), str2 )
end

test['should middle of string'] = function()
  str1 = "this is a test"
  str2 = "is a"
  should.be_equal( str1:mid(5, 4), str2 )
end

test['should end of string'] = function()
  str1 = "this is a test"
  str2 = "test"
  should.be_equal( str1:mid(10, 4), str2 )
end

test['should return init after end string'] = function()
  str1 = "this is a test"
  str2 = "is a test"
  should.be_equal( str1:mid(5, -1), str2 )
end

return test
