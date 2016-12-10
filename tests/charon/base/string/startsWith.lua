local test = {}

test['should true for compatible start string'] = function()
  str = "this is a test"
  assert( str:startsWith("this is") == true)
end

test['should false for incompatible start string'] = function()
  str = "this is a test"
  assert( str:startsWith("other is") == false)
end

return test
