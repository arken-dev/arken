local test = {}

test['should swap a smaller string for greater string'] = function()
  local str = "this is a test"
  assert( str:swap(" is ", " is not ") == "this is not a test" )
end

test['should swap a greater string for smaller string'] = function()
  local str = "this is not a test"
  assert( str:swap(" is not ", " is ") == "this is a test" )
end

test['should swap a string for empty string'] = function()
  local str = "this is not a test"
  assert( str:swap(" not ", " ") == "this is a test" )
end

test['should preserve string not valid string'] = function()
  local str = "this is not a test"
  assert( str:swap("yes", "") == "this is not a test" )
end

return test
