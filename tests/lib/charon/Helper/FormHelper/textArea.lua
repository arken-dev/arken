local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html_input = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:textArea('id')
  local html = [[<textarea id="form_id" name="form[id]" style="width:250px;height:65px">1234</textarea>]]
  assert( result == html, result )
end

test.should_return_html_input_with_onblur = function()
  local data       = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form       = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result     = form:textArea('id', { onblur = "alert('here')" })
  local html       = [[<textarea id="form_id" name="form[id]" style="width:250px;height:65px" onblur="alert('here')">1234</textarea>]]
  assert( result == html, result )
end

test.should_return_html_input_with_onfocus = function()
  local data       = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form       = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result     = form:textArea('id', { onfocus = "alert('here')" })
  local html       = [[<textarea id="form_id" name="form[id]" style="width:250px;height:65px" onfocus="alert('here')">1234</textarea>]]
  assert( result == html, result )
end

test.should_return_html_input_with_onkeypress = function()
  local data       = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form       = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result     = form:textArea('id', { onkeypress = "alert('here')" })
  local html       = [[<textarea id="form_id" name="form[id]" style="width:250px;height:65px" onkeypress="alert('here')">1234</textarea>]]
  assert( result == html, result )
end

test.should_return_html_input_with_onkeyup = function()
  local data       = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form       = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result     = form:textArea('id', { onkeyup = "alert('here')" })
  local html       = [[<textarea id="form_id" name="form[id]" style="width:250px;height:65px" onkeyup="alert('here')">1234</textarea>]]
  assert( result == html, result )
end

return test
