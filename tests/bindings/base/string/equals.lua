local test = {}

test['should return true for equal userdata and string'] = function()
  local str1 = string.new('hello world')
  local str2 = 'hello world'
  assert( str1:equals(str2) == true )
end

test['should return true for equal string and userdata'] = function()
  local str1 = string.new('hello world')
  local str2 = 'hello world'
  assert( str1:equals(str2) == true )
end

test['should return true for equal string and string'] = function()
  local str1 = 'hello world'
  local str2 = 'hello world'
  assert( str1:equals(str2) == true )
end

test['should return true for equal userdata and userdata'] = function()
  local str1 = string.new('hello world')
  local str2 = string.new('hello world')
  assert( str1:equals(str2) == true )
end

test['should return false for diff strings'] = function()
  local str1 = 'hello world'
  local str2 = 'hello world!'
  assert( str1:equals(str2) == false )
end

test['should return false for diff userdata'] = function()
  local str1 = string.new('hello world')
  local str2 = string.new('hello world!')
  assert( str1:equals(str2) == false )
end

test['should return false for diff string and userdata'] = function()
  local str1 = 'hello world'
  local str2 = string.new('hello world!')
  assert( str1:equals(str2) == false )
end

test['should return false for diff userdata and string'] = function()
  local str1 = string.new('hello world')
  local str2 = 'hello world!'
  assert( str1:equals(str2) == false )
end

test['should return error for diff userdata and boolean'] = function()
  local str1 = string.new('hello world')
  local flag, message = pcall(str1.equals, str1, false)
  assert( flag == false )
  assert( message:contains('string expected, got boolean') == true, message )
end

test['should return error for diff string and boolean'] = function()
  local str1 = 'hello world'
  local flag, message = pcall(str1.equals, str1, false)
  assert( flag == false )
  assert( message:contains('string expected, got boolean') == true, message )
end

return test
