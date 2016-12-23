local test = {}

test['should be zero for first string'] = function()
  local str = "this is a test"
  assert( str:indexOf('t') == 0 )
end

test['should be zero for second string'] = function()
  local str = "this is a test"
  assert( str:indexOf('h') == 1 )
end

test['should be zero for third string'] = function()
  local str = "this is a test"
  assert( str:indexOf('i') == 2 )
end

test['should be zero for fourth string'] = function()
  local str = "this is a test"
  assert( str:indexOf('s') == 3 )
end

test['should be zero for last string'] = function()
  local str = "this is a test\n"
  assert( str:indexOf('\n') == 14 )
end

test['should return -1 if string not found'] = function()
  local str = "this is a test\n"
  assert( str:indexOf('other') == -1 )
end

return test
