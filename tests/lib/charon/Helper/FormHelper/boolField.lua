local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html_bool_field = function()
  local controller = {controller_name = 'form', action_name = 'save'}
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="radio" name="form[canceled]" value="true"  > yes <input type="radio" name="form[canceled]" value="false"  > not ]]
  local result = form:boolField('canceled')
  assert( result == html, result )
end

test.should_return_html_bool_field_true_checked = function()
  local controller = {controller_name = 'form', action_name = 'save'}
  local data   = { canceled = true }
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="radio" name="form[canceled]" value="true" checked > yes <input type="radio" name="form[canceled]" value="false"  > not ]]
  local result = form:boolField('canceled')
  assert( result == html, result )
end

test.should_return_html_bool_field_false_checked = function()
  local controller = {controller_name = 'form', action_name = 'save'}
  local data   = { canceled = false }
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="radio" name="form[canceled]" value="true"  > yes <input type="radio" name="form[canceled]" value="false" checked > not ]]
  local result = form:boolField('canceled')
  assert( result == html, result )
end

test.should_return_html_bool_field_blank_option = function()
  local controller = {controller_name = 'form', action_name = 'save'}
  local data   = { canceled = false }
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="radio" name="form[canceled]" value="true"  > yes <input type="radio" name="form[canceled]" value="false" checked > not <input type="radio" name="form[canceled]" value=""  > both ]]
  local result = form:boolField('canceled', { blank = 'both' } )
  assert( result == html, result )
end

test.should_return_html_bool_field_blank_option_checked = function()
  local controller = {controller_name = 'form', action_name = 'save'}
  local data   = { canceled = '' }
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="radio" name="form[canceled]" value="true"  > yes <input type="radio" name="form[canceled]" value="false"  > not <input type="radio" name="form[canceled]" value="" checked > both ]]
  local result = form:boolField('canceled', { blank = 'both' } )
  assert( result == html, result )
end

test.should_return_html_bool_field_nil_option_checked = function()
  local controller = {controller_name = 'form', action_name = 'save'}
  local data   = { canceled = nil }
  local form   = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html   = [[<input type="radio" name="form[canceled]" value="true"  > yes <input type="radio" name="form[canceled]" value="false"  > not <input type="radio" name="form[canceled]" value="" checked > both ]]
  local result = form:boolField('canceled', { blank = 'both' } )
  assert( result == html, result )
end

return test
