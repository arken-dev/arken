local test = {}

test['should replace multiple string with equal size'] = function()
  local str = "this is a test"
  local res = str:replace("is", "**")
  assert( res == "th** ** a test", res )
end

test['should replace multiple string with diff size'] = function()
  local str = "this is a test"
  local res = str:replace("is", "*")
  assert( res == "th* * a test", res )
end

test['should replace multiple string with major size'] = function()
  local str = "this is a test"
  local res = str:replace("is", "***")
  assert( res == "th*** *** a test", res )
end

test['should replace multiple string with equal size not end'] = function()
  local str = "this is"
  local res = str:replace("is", "**")
  assert( res == "th** **", res )
end

test['should replace multiple string with equal size not end'] = function()
  local str = "this is"
  local res = str:replace("is", "*")
  assert( res == "th* *", res )
end

test['should replace multiple string with major size not end'] = function()
  local str = "this is"
  local res = str:replace("is", "***")
  assert( res == "th*** ***", res )
end

test['should replace a all "is"'] = function()
  local str = "this is a test"
  local res = str:replace("is", "is not")
  assert( res == "this not is not a test", res )
end

test['should replace a smaller string for greater string'] = function()
local str = "this is a test"
assert( str:replace(" is ", " is not ") == "this is not a test" )
end

test['should replace a greater string for smaller string'] = function()
  local str = "this is not a test"
  assert( str:replace(" is not ", " is ") == "this is a test" )
end

test['should replace a string for empty string'] = function()
  local str = "this is not a test"
  assert( str:replace(" not ", " ") == "this is a test" )
end

test['should preserve string not valid string'] = function()
  local str = "this is not a test"
  assert( str:replace("yes", "") == "this is not a test" )
end

test['should preserve string not valid string with replace char'] = function()
  local str = "this is not a test"
  assert( str:replace("yes", "*") == "this is not a test" )
end

test['should replace a single char'] = function()
  local str   = "this is a test"
  local value = str:replace("i", "|")
  assert( value == "th|s |s a test", value )
end

test['should replace a two char'] = function()
  local str   = "this is a test"
  local value = str:replace("is", "|!")
  assert( value == "th|! |! a test", value )
end

test['should replace a single char start'] = function()
  local str   = "this is a test"
  local value = str:replace("i", "|", 6)
  assert( value == "this |s a test", value )
end

test['should replace a string start in beforeposition'] = function()
  local str   = "this is a test"
  local value = str:replace("is", "at", 5)
  assert( value == "this at a test", value )
end

test['should replace a string start in exact position'] = function()
  local str   = "this is a test"
  local value = str:replace("is", "at", 6)
  assert( value == "this at a test", value )
end

test['should replace a string negative start'] = function()
  local str   = "this is a test"
  local value = str:replace("is", "at", -10)
  assert( value == "this at a test", value )
end

test['should replace a ext name file with negative start'] = function()
  local str   = "image.jpg"
  local value = str:replace("jpg", "gif", -3)
  assert( value == "image.gif", value )
end

test['should replace a ext name file'] = function()
  local str   = "image.jpg"
  local value = str:replace("jpg", "gif")
  assert( value == "image.gif", value )
end

test['should replace a ext name file char with negative start'] = function()
  local str   = "image.jpG"
  local value = str:replace("G", "g", -1)
  assert( value == "image.jpg", value )
end

return test
