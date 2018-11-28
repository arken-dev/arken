local test = {}

test['should before of string'] = function()
  local str1 = "this is a test"
  local str2 = "this is"
  assert( str1:mid(1, 7) == str2 )
end

test['should middle of string'] = function()
  local str1 = "this is a test"
  local str2 = "is a"
  assert( str1:mid(6, 4) == str2 )
end

test['should end of string'] = function()
  local str1 = "this is a test"
  local str2 = "test"
  assert( str1:mid(11, 4) == str2 )
end

test['should return init after end string'] = function()
  local str1 = "this is a test"
  local str2 = "is a test"
  assert( str1:mid(6, -1) == str2 )
end

test['should return empty string if start is 0'] = function()
  local str = "this is a test"
  assert( str:mid(0, 1) == "", 'resultado ' .. str:mid(1, 1))
end

test['should return empty string if start is major 0 and length is 0'] = function()
  local str = ""
  assert( str:mid(3, 0) == "", 'resultado ' .. str:mid(3, 0))
end

test['should return empty string if index is major and len is negative'] = function()
  local str = "/"
  assert( str:mid(4, -4) == "", 'resultado ' .. str:mid(4, -4))
end

test['should return empty string if index is a equal string size and pos is -1'] = function()
  local str = 'abcd'
  assert( str:mid(5, -1) == "" )
end

return test
