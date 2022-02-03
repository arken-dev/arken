local test = {}

test['should return 25.78 for 25.775'] = function()
  local result = math.round(25.775, 2)
  assert( result == 25.78, result)
end

return test
