local FormHelper = require('arken.Helper.FormHelper')
local test = {}

test.should_return_url_string = function()
  local form = FormHelper.new()
  assert( form:url("/app/redirect") == "/app/redirect" )
end

test.should_return_action_and_controller_ommitted = function()
  local controller = { actionName = nil }
  local form = FormHelper.new{ controller = {} }
  local result = form:url({})
  assert(  result == "index/index", result )
end

test.should_return_action_and_controller_ommitted_and_inside_controller_form = function()
  local controller = { actionName = nil }
  local form = FormHelper.new{ controller = {controllerName = 'form'} }
  local result = form:url({})
  assert( result == "form/index", result )
end

test.should_return_index_if_ommitted = function()
  local controller = { actionName = nil }
  local form = FormHelper.new{ controller = controller }
  local result = form:url({controller = "redirect"})
  assert( result == "redirect/index", result )
end

test.should_return_action = function()
  local controller = { actionName = 'save' }
  local form   = FormHelper.new{ controller = controller }
  local result = form:url({controller = "redirect"})
  assert( result == "redirect/save", result )
end

test.should_return_action = function()
  local controller  = { actionName = 'save', prefix = '/app' }
  local form        = FormHelper.new{ controller = controller }
  local result      = form:url( {controller = "redirect"} )
  assert( result == '/app/redirect/save', result )
end

test.should_return_url_with_params = function()
  local controller = { actionName = 'save' }
  local form   = FormHelper.new{ controller = controller }
  local result = form:url({controller = "redirect", name = "Alex", id = 1})
  assert( result == "redirect/save?id=1&name=Alex", result )
end

return test
