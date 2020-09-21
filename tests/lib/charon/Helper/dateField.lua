local helper = require('arken.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:dateField("date", "2016/01/01")
  local html   = [[<input type="text" id="date" name="date" value="2016/01/01" onfocus="inputFocus( this )" onblur="inputBlur( this )" onkeyup="format_invert(this, '**/**/****')" onkeypress="inputPress( this, event )" style="width:80px;text-align:right"/><script>jQuery.calendar('#date')</script>]]
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

return test
