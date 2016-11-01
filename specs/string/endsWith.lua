local test = {}

test['should true for a extension image'] = function()
  str1 = "photo.png"
  should.be_valid( str1:endsWith(".png") )
end

test['should false for a invalid extension image'] = function()
  str1 = "photo.png"
  should.not_valid( str1:endsWith(".gif") )
end

test['should true for a extension image with new line'] = function()
  str1 = "photo.png\n"
  should.be_valid( str1:endsWith(".png\n") )
end

test['should true for a extension image with return and new line'] = function()
  str1 = "photo.png\r\n"
  should.be_valid( str1:endsWith(".png\r\n") )
end

return test
