local should = require "test.should"
local test   = {}

test['should insert in before string'] = function()
  str1 = "this is a test"
  str2 = "before this is a test"
  should.be_equal( str1:insert(0, "before "), str2 )
end

test['should insert in middle string'] = function()
  str1 = "this is a test"
  str2 = "this is middle a test"
  should.be_equal( str1:insert(8, "middle "), str2 )
end

test['should insert in end string'] = function()
  str1 = "this is a test"
  str2 = "this is a test end"
  should.be_equal( str1:insert(14, " end"), str2 )
end

return test
