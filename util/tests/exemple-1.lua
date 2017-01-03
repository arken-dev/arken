local test = {}

test.should_assert_true = function()
  local value = true
  assert(  value == true )
end

return test
