local test = {}

test['should before string'] = function()
  local str1 = "this is a test"
  local str2 = "this is"
  assert( str1:left(7) == str2 )
end

test['should before string with new line'] = function()
  local str1 = "\nthis is a test"
  local str2 = "\nthis is"
  assert( str1:left(8) == str2 )
end

return test
