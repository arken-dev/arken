local should = require "test.should"
local test   = {}

test['should change spaces to underscore'] = function()
  local str1 = "this is a test"
  local str2 = "this_is_a_test"
  should.be_equal( str1:underscore(), str2 )
end

test['should change upcar case to underscore and lower case'] = function()
  local str1 = "ThisIsATest"
  local str2 = "this_is_a_test"
  should.be_equal( str1:underscore(), str2 )
end

test['should change upper case and spaces to one underscore and lower case'] = function()
  local str1 = "This Is A Test"
  local str2 = "this_is_a_test"
  should.be_equal( str1:underscore(), str2 )
end

return test
