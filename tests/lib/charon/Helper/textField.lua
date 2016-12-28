local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:textField("name", "John")
  local html   = [[<input type="text" id="name" name="name" value="John" style="width:250px" />]]
  assert( result == html, result )
end

test.should_return_html_with_onblur = function()
  local result = helper:textField("name", "John", { onblur = 'alert("here !!!")' } )
  local html   = [[<input type="text" id="name" name="name" value="John" style="width:250px" onblur="alert(\"here !!!\")"  />]]
  assert( result == html, result )
end

test.should_return_html_with_onfocus = function()
  local result = helper:textField("name", "John", { onfocus = 'alert("here !!!")' } )
  local html   = [[<input type="text" id="name" name="name" value="John" style="width:250px" onfocus="alert(\"here !!!\")"  />]]
  assert( result == html, result )
end

test.should_return_html_with_onkeypress = function()
  local result = helper:textField("name", "John", { onkeypress = 'alert("here !!!")' } )
  local html   = [[<input type="text" id="name" name="name" value="John" style="width:250px" onkeypress="alert(\"here !!!\")"  />]]
  assert( result == html, result )
end

test.should_return_html_with_onkeyup = function()
  local result = helper:textField("name", "John", { onkeyup = 'alert("here !!!")' } )
  local html   = [[<input type="text" id="name" name="name" value="John" style="width:250px" onkeyup="alert(\"here !!!\")"  />]]
  assert( result == html, result )
end

return test
