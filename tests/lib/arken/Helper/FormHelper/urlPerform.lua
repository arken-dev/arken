local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_url_string = function()
  local controller = {controllerName = 'form', actionName = 'save', prefix = ''}
  local form = FormHelper.new{ controller = controller }
  assert( form:urlPerform() == "/form/savePerform", form:urlPerform() )
end

test.should_return_url_string_with_prefix = function()
  local controller = {controllerName = 'form', actionName = 'save', prefix = '/app'}
  local form = FormHelper.new{ controller = controller }
  assert( form:urlPerform() == "/app/form/savePerform", form:urlPerform() )
end

return test
