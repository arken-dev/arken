local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_html_date_field = function()
  local data = { data = '01/01/2016' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_data" name="form[data]" value="01/01/2016" style="width:80px;text-align:right" onblur="inputBlur( this )" onfocus="inputFocus( this )" onkeypress="inputPress( this, event )" onkeyup="format_invert(this, '**/**/****')" /><script>jQuery.calendar('#form_data')</script>]]
  local result = form:dateField('data')
  assert( result == html, result )
end

test.should_return_html_date_field_with_american_format  = function()
  local data = { data = '2016/01/01' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_data" name="form[data]" value="01/01/2016" style="width:80px;text-align:right" onblur="inputBlur( this )" onfocus="inputFocus( this )" onkeypress="inputPress( this, event )" onkeyup="format_invert(this, '**/**/****')" /><script>jQuery.calendar('#form_data')</script>]]
  local result = form:dateField('data')
  assert( result == html, result )
end

test.should_return_html_date_field_with_american_format_with_trace  = function()
  local data = { data = '2016-01-01' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_data" name="form[data]" value="01-01-2016" style="width:80px;text-align:right" onblur="inputBlur( this )" onfocus="inputFocus( this )" onkeypress="inputPress( this, event )" onkeyup="format_invert(this, '**/**/****')" /><script>jQuery.calendar('#form_data')</script>]]
  local result = form:dateField('data')
  assert( result == html, result )
end

return test
