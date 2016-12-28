local helper = require('charon.Helper')
local test   = {}

test.should_return_html_with_name_of_data = function()
  helper.controller = {}
  helper.controller.form = { id = 134, description = 'my description' }
  local form   = helper:formCreate('form')
  local result = form:hiddenField('id')
  local html   = [[<input type="hidden" id="form_id" name="form[id]" value="134">]]
  assert( result == html, result )
end

test.should_return_html_with_data = function()
  local form   = helper:formCreate({ id = 134, description = 'my description' })
  local result = form:hiddenField('id')
  local html   = [[<input type="hidden" id="form_id" name="form[id]" value="134">]]
  assert( result == html, result )
end

return test
