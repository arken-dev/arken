local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:intField("id", "1234")
  local html   = [[<input type="text" id="id" name="id" value="1.234" onfocus="inputFocus( this )" onblur="inputBlur( this )" onkeyup="format_invert(this, '***.***.***')" onkeypress="inputPress( this, event )" style="width:95px;text-align:right"/>]]
  assert( result == html, result:escapeHtml() )
end

return test
