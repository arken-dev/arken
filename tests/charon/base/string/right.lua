local test = {}

test['should end of string'] = function()
  local str1 = "this is a test"
  local str2 = "test"
  assert( str1:right(5) == str2 )
end

test['should before string with new line'] = function()
  local str1 = "this is a test\n"
  local str2 = "test\n"
  assert( str1:right(6) == str2 )
end

return test
