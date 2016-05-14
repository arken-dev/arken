local should = require "test.should"
local test   = {}

test['should be 13 for first string'] = function()
  str = "this is a test"
  should.be_equal( str:lastIndexOf('t'), 13 )
end

test['should be zero for second string'] = function()
  str = "this is a test"
  should.be_equal( str:lastIndexOf('h'), 1 )
end

test['should be zero for third string'] = function()
  str = "this is a test"
  should.be_equal( str:lastIndexOf('i'), 5 )
end

test['should be zero for fourth string'] = function()
  str = "this is a test"
  should.be_equal( str:lastIndexOf('s'), 12 )
end

test['should be zero for last string'] = function()
  str = "this is a test\n"
  should.be_equal( str:lastIndexOf('\n'), 14 )
end

return test
