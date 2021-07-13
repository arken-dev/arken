local test = {}

test['should be 13 for first string'] = function()
  local str = "this is a test"
  assert( str:lastIndexOf('t') == 14 )
end

test['should be zero for second string'] = function()
  local str = "this is a test"
  assert( str:lastIndexOf('h') == 2 )
end

test['should be zero for third string'] = function()
  local str = "this is a test"
  assert( str:lastIndexOf('i') == 6 )
end

test['should be zero for fourth string'] = function()
  local str = "this is a test"
  assert( str:lastIndexOf('s') == 13 )
end

test['should be zero for last string'] = function()
  local str = "this is a test\n"
  assert( str:lastIndexOf('\n') == 15 )
end

test['should be one negative for last string'] = function()
  local str = "this is a test\n"
  assert( str:lastIndexOf('other') == -1 )
end

return test
