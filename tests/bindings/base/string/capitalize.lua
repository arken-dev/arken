local test = {}

test['should upper first string'] = function()
  local str = "this is a test"
  local res = str:capitalize()
  assert( str:capitalize() == "This is a test", res )
end

test['should keep first string upper'] = function()
  local str = "This is a test"
  local res = str:capitalize()
  assert( str:capitalize() == "This is a test", res )
end

test['should keep blank string first'] = function()
  local str = " This is a test"
  local res = str:capitalize()
  assert( str:capitalize() == " This is a test", res )
end

return test
