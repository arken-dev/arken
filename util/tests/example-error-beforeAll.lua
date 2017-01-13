local test = {}

test.beforeAll = function()
  error("error before all...")
end

test.my_test_ok= function()
  local value = true
  assert( value == true )
end

return test
