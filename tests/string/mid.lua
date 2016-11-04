local test = {}

test['should before of string'] = function()
  str1 = "this is a test"
  str2 = "this is"
  assert( str1:mid(0, 7) == str2 )
end

test['should middle of string'] = function()
  str1 = "this is a test"
  str2 = "is a"
  assert( str1:mid(5, 4) == str2 )
end

test['should end of string'] = function()
  str1 = "this is a test"
  str2 = "test"
  assert( str1:mid(10, 4) == str2 )
end

test['should return init after end string'] = function()
  str1 = "this is a test"
  str2 = "is a test"
  assert( str1:mid(5, -1) == str2 )
end

return test
