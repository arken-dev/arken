local helper = require('arken.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:floatField("price", "1234.35")
  local html   = [[<input type="text" id="price" name="price" value="1.234,35" onfocus="inputFocus( this )" onblur="inputBlur( this )" onkeyup="format_invert(this, '**.***.***,**')" onkeypress="inputPress( this, event )" style="width:95px;text-align:right"/>]]
  assert( result == html, result:replace(' ', '*') .. html:replace(' ', '*') )
end

return test
