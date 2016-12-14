local test = {}

test['should replace a smaller string for greater string'] = function()
  local str = "this is a test"
  assert( str:replace(" is ", " is not ") == "this is not a test" )
end

test['should replace a greater string for smaller string'] = function()
  local str = "this is not a test"
  assert( str:replace(" is not ", " is ") == "this is a test" )
end

test['should replace a string for empty string'] = function()
  local str = "this is not a test"
  assert( str:replace(" not ", " ") == "this is a test" )
end

test['should preserve string not valid string'] = function()
  local str = "this is not a test"
  assert( str:replace("yes", "") == "this is not a test" )
end

return test
