local test = {}

test['should insert in before string'] = function()
  str1 = "this is a test"
  str2 = "before this is a test"
  assert( str1:insert(1, "before ") == str2 )
end

test['should insert in middle string'] = function()
  str1 = "this is a test"
  str2 = "this is middle a test"
  assert( str1:insert(9, "middle ") == str2 )
end

test['should insert in end string'] = function()
  str1 = "this is a test"
  str2 = "this is a test end"
  assert( str1:insert(15, " end") == str2 )
end

return test
