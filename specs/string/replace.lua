local should = require "test.should"
local test   = {}

test['should replace space dot'] = function()
  str1 = "this is a test"
  str2 = "this.is.a.test"
  should.be_equal( str1:replace(" ", "."), str2 )
end

test['should replace letter before and end'] = function()
  str1 = "this is a test"
  str2 = "-his is a -es-"
  should.be_equal( str1:replace("t", "-"), str2 )
end

test['should replace letter before for empty after'] = function()
  str1 = "this is a test"
  str2 = "his is a es"
  should.be_equal( str1:replace("t", ""), str2 )
end

return test
