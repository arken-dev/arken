local should = require "test.should"
local test   = {}

test['should upper first string'] = function()
  str1 = "this is a test"
  str2 = "This is a test"
  should.be_equal( str1:capitalize(), str2 )
end

test['should keep first string upper'] = function()
  str1 = "This is a test"
  str2 = "This is a test"
  should.be_equal( str1:capitalize(), str2 )
end

test['should keep blank string first'] = function()
  str1 = " This is a test"
  str2 = " This is a test"
  should.be_equal( str1:capitalize(), str2 )
end

return test
