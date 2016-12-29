local test = {}

test['should replace a smaller string for greater string'] = function()
  local str = "this is a test"
  assert( str:replaceAll(" is ", " is not ") == "this is not a test" )
end

test['should replace a greater string for smaller string'] = function()
  local str = "this is not a test"
  assert( str:replaceAll(" is not ", " is ") == "this is a test" )
end

test['should replace a string for empty string'] = function()
  local str = "this is not a test"
  assert( str:replaceAll(" not ", " ") == "this is a test" )
end

test['should preserve string not valid string'] = function()
  local str = "this is not a test"
  assert( str:replaceAll("yes", "") == "this is not a test" )
end

return test
