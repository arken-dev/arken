local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:dateField("date", "2016/01/01")
  local html   = [[<input type="text" id="date" name="date" value="2016/01/01" style="width:80px;text-align:right" onblur="inputBlur( this )"  onfocus="inputFocus( this )"  onkeypress="inputPress( this, event )"  onkeyup="format_invert(this, '**/**/****')"  /><script>jQuery.calendar('#date')</script>]]
  assert( result == html, result )
end

return test
