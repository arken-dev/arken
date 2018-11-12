local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html_input = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1.234,00" style="width:95px;text-align:right" onblur="inputBlur( this )" onfocus="inputFocus( this )" onkeypress="inputPress( this, event )" onkeyup="format_invert(this, '**.***.***,**')" />]]
  local result = form:floatField('id')
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

return test
