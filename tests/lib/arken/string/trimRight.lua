local test = {}

test['should remove white spaces'] = function()
  local str1 = "   this is a test   "
  local str2 = "   this is a test"
  assert( str1:trimRight() == str2 )
end

test['should remove left white spaces'] = function()
  local str1 = "   this is a test"
  local str2 = "   this is a test"
  assert( str1:trimRight() == str2 )
end

test['should remove right white spaces'] = function()
  local str1 = "this is a test      "
  local str2 = "this is a test"
  assert( str1:trimRight() == str2 )
end

test['should remove new lines and white spaces'] = function()
  local str1 = "\n \nthis is a test\n \n"
  local str2 = "\n \nthis is a test"
  assert( str1:trimRight() == str2 )
end

test['should empty string for new line char'] = function()
  local str1 = "\n"
  local str2 = ""
  assert( str1:trimRight() == str2, str1 )
end

test['should empty string for return and new line chars'] = function()
  local str1 = "\r\n"
  local str2 = ""
  assert( str1:trimRight() == str2 )
end

test['should empty string for empty string'] = function()
  local str1 = ""
  local str2 = ""
  assert( str1:trimRight() == str2 )
end

test['should string with one char'] = function()
  local str1 = "("
  local str2 = "("
  assert( str1:trimRight() == str2, str1:trimRight() )
end

test['should string with one char with space right'] = function()
  local str1 = "( "
  local str2 = "("
  assert( str1:trimRight() == str2, str1:trimRight() )
end

test['should string with one char with space left'] = function()
  local str1 = " ("
  local str2 = " ("
  assert( str1:trimRight() == str2, str1:trimRight() )
end

test['should string with one char with space spaces'] = function()
  local str1 = " ("
  local str2 = " ("
  assert( str1:trimRight() == str2, str1:trimRight() )
end

return test
