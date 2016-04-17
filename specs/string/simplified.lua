local should = require "test.should"
local test   = {}

test['should remove blanks from the beginning and the end'] = function()
  str1 = "  this is a test  "
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove one char blank from the beginning and the end'] = function()
  str1 = " this is a test "
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove blanks throughout the string'] = function()
  str1 = "   this   is   a   test   "
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove break line throughout the string'] = function()
  str1 = "\nthis\nis\na\ntest\n"
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove tab throughout the string'] = function()
  str1 = "\tthis\tis\ta\ttest\t"
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove blank, break line and tab throughout the string'] = function()
  str1 = " \n\t this \t\n is \t\n a \t\n test \t\n "
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )

  str1 = "\n \tthis\t \nis\t \na\t \ntest\t \n"
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

test['should remove tab throughout the string'] = function()
  str1 = "　this　is　a　test　"
  str2 = "this is a test"
  should.be_equal( str1:simplified(), str2 )
end

return test
