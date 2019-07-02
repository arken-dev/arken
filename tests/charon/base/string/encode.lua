local test = {}

test['should encode and decode'] = function()
  local val1  = "não"
  local val2 = val1:encode('ISO 8859-1')
  local val3 = val2:decode('ISO 8859-1')
  assert( val3  == "não", string.format("%s %s %s %i %i %i", val1, val2, val3, #val1, #val2, #val3) )
end

return test
