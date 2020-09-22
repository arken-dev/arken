local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_url_string = function()
  request = {}
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller }
  assert( form:buildId('id') == "form_id", form:buildId('id'))
end

return test
