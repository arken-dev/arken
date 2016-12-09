local test = {}

test['should true for a extension image'] = function()
  str1 = "photo.png"
  assert( str1:endsWith(".png") == true )
end

test['should false for a invalid extension image'] = function()
  str1 = "photo.png"
  assert( str1:endsWith(".gif") == false )
end

test['should true for a extension image with new line'] = function()
  str1 = "photo.png\n"
  assert( str1:endsWith(".png\n") == true )
end

test['should true for a extension image with return and new line'] = function()
  str1 = "photo.png\r\n"
  assert( str1:endsWith(".png\r\n") == true )
end

return test
