local should = require "test.should"
local test   = {}

test['should remove blanks from the beginning and the end'] = function()
  local str1 = "  this is a test  "
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove one char blank from the beginning and the end'] = function()
  local str1 = " this is a test "
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove blanks throughout the string'] = function()
  local str1 = "   this   is   a   test   "
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove break line throughout the string'] = function()
  local str1 = "\nthis\nis\na\ntest\n"
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove tab throughout the string'] = function()
  local str1 = "\tthis\tis\ta\ttest\t"
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove blank, break line and tab throughout the string'] = function()
  local str1 = " \n\t this \t\n is \t\n a \t\n test \t\n "
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
  local str1 = "\n \tthis\t \nis\t \na\t \ntest\t \n"
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove tab throughout the string'] = function()
  local str1 = "　this　is　a　test　"
  local str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should apply sequential methodo'] = function()
  local str1 = "　this　is　a　test　"
  local str2 = "this is a test"
  should.be_equal( str1:simplified():simplified(), str2 )
end

test['should return empty string for many invalid chars'] = function()
  local str = "   "
  should.be_equal( str:simplified(), "" )

  str = "\n\r "
  should.be_equal( str:simplified(), "" )
end

return test
