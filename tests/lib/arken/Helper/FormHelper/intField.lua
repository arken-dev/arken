local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_html_input = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1234" style="width:95px;text-align:right" onblur="inputBlur( this )" onfocus="inputFocus( this )" onkeypress="inputPress( this, event )" onkeyup="format_invert(this, '***.***.***')" />]]
  local result = form:intField('id')
  assert( result == html, result )
end

return test
