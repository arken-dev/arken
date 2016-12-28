local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html_input = function()
  local data = { id = '1234' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local html = [[<input type="hidden" id="form_id" name="form[id]" value="1234">]]
  assert( form:hiddenField('id') == html )
end

return test
