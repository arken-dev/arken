local test = {}

test['should contains before of string'] = function()
  str1 = "this is a test"
  assert( str1:contains('this') == true)
end

test['should contains end of string'] = function()
  str1 = "this is a test"
  assert( str1:contains('test') == true)
end

test['should contains middle of string'] = function()
  str1 = "this is a test"
  assert( str1:contains('is a') == true)
end

test['should contains middle and new line of string'] = function()
  str1 = "this is\na test"
  assert( str1:contains('is\na') == true)
end

return test
