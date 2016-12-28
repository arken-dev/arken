local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:textArea("name", "John")
  local html   = [[<textarea id="name" name="name" style="width:250px;height:65px">John</textarea>]]
  assert( result == html, result )
end

test.should_return_html_with_onblur = function()
  local result = helper:textArea("name", "John", { onblur = "alert('here !!!')" } )
  local html   = [[<textarea id="name" name="name" style="width:250px;height:65px" onblur="alert('here !!!')" >John</textarea>]]
  assert( result == html, result )
end

test.should_return_html_with_onfocus = function()
  local result = helper:textArea("name", "John", { onfocus = "alert('here !!!')" } )
  local html   = [[<textarea id="name" name="name" style="width:250px;height:65px" onfocus="alert('here !!!')" >John</textarea>]]
  assert( result == html, result )
end

test.should_return_html_with_onkeypress = function()
  local result = helper:textArea("name", "John", { onkeypress = "alert('here !!!')" } )
  local html   = [[<textarea id="name" name="name" style="width:250px;height:65px" onkeypress="alert('here !!!')" >John</textarea>]]
  assert( result == html, result )
end

test.should_return_html_with_onkeyup = function()
  local result = helper:textArea("name", "John", { onkeyup = "alert('here !!!')" } )
  local html   = [[<textarea id="name" name="name" style="width:250px;height:65px" onkeyup="alert('here !!!')" >John</textarea>]]
  assert( result == html, result )
end

return test
