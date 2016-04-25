local should = require "test.should"
local test   = {}

test['should before string'] = function()
  str1 = "this is a test"
  str2 = "this is"
  should.be_equal( str1:left(7), str2 )
end

test['should before string with new line'] = function()
  str1 = "\nthis is a test"
  str2 = "\nthis is"
  should.be_equal( str1:left(8), str2 )
end


return test
