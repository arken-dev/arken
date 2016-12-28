local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:floatField("price", "1234.35")
  local html   = [[<input type="text" id="price" name="price" value="1234.35" style="width:95px;text-align:right" onblur="inputBlur( this )"  onfocus="inputFocus( this )"  onkeypress="inputPress( this, event )"  onkeyup="format_invert(this, '**.***.***,**')"  />]]
  assert( result == html, result )
end

return test
