local should = require "test.should"
local test   = {}

test['should append new string'] = function()
  str1 = "this is a test"
  str2 = "this is a test"
  should.be_equal( str1:append(str2), "this is a testthis is a test" )
end

test['should tree strings'] = function()
  str1 = "this is a test"
  str2 = " "
  str3 = "this is a test"
  should.be_equal( str1:append(str2):append(str3), "this is a test this is a test" )
end


return test
