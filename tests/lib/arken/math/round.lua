local test = {}

test['should return 25.78 for 25.775'] = function()
  local result = math.round(25.775, 2)
  assert( result == 25.78, result)
end

test['should return 150.55 for 150.55'] = function()
  local result = math.round(150.55, 2)
  assert( result == 150.55, result)
end

return test
