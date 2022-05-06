local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_html_input = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html = [[<input type="text" id="form_id" name="form[id]" value="1234" />]]
  assert( form:textField('id') == html, form:textField('id') )
end

test.should_return_html_input_with_onblur = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1234" onblur="alert('here')" />]]
  local result = form:textField('id', { onblur = "alert('here')" })
  assert( result == html, result )
end

test.should_return_html_input_with_onfocus = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1234" onfocus="alert('here')" />]]
  local result = form:textField('id', { onfocus = "alert('here')" })
  assert( result == html, result )
end

test.should_return_html_input_with_onkeypress = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1234" onkeypress="alert('here')" />]]
  local result = form:textField('id', { onkeypress = "alert('here')" })
  assert( result == html, result )
end

test.should_return_html_input_with_onkeyup = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1234" onkeyup="alert('here')" />]]
  local result = form:textField('id', { onkeyup = "alert('here')" })
  assert( result == html, result )
end

test.should_return_html_input_with_onkeypress_and_onkeyup = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="text" id="form_id" name="form[id]" value="1234" onkeyup="alert('here')" onkeypress="alert('here')" />]]
  local result = form:textField('id', { onkeypress = "alert('here')" , onkeyup = "alert('here')" })
  assert( result == html, result )
end

return test
