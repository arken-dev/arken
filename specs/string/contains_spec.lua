local should = require "test.should"
local test   = {}

test['should contains before of string'] = function()
  str1 = "this is a test"
  should.be_valid( str1:contains('this') )
end

test['should contains end of string'] = function()
  str1 = "this is a test"
  should.be_valid( str1:contains('test') )
end

test['should contains middle of string'] = function()
  str1 = "this is a test"
  should.be_valid( str1:contains('is a') )
end

test['should contains middle and new line of string'] = function()
  str1 = "this is\na test"
  should.be_valid( str1:contains('is\na') )
end

return test
