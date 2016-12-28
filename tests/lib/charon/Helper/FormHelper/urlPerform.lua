local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_url_string = function()
  request = {}
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ controller = controller }
  assert( form:urlPerform() == "/form/savePerform", form:urlPerform() )
end

return test
