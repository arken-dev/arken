local test = {}

test['should append new string'] = function()
  local str1 = "this is a test"
  local str2 = "this is a test"
  assert( str1:append(str2) == "this is a testthis is a test" )
end

test['should tree strings'] = function()
  local str1 = "this is a test"
  local str2 = " "
  local str3 = "this is a test"
  assert( str1:append(str2):append(str3) == "this is a test this is a test" )
end

test['should append empty string'] = function()
  local str1 = "this is a test"
  local str2 = ""
  assert( str1:append(str2) == "this is a test" )

  local str1 = ""
  local str2 = "this is a test"
  assert( str1:append(str2) == "this is a test" )
end

return test
