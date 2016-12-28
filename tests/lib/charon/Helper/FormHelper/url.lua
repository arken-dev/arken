local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_url_string = function()
  local form = FormHelper.new()
  assert( form:url("/app/redirect") == "/app/redirect" )
end

test.should_return_action_and_controller_ommitted = function()
  request = {}
  local controller = { action_name = nil }
  local form = FormHelper.new{ controller = {} }
  assert( form:url({}) == "/index/index", form:url({controller = "index"}) )
end

test.should_return_action_and_controller_ommitted_and_inside_controller_form = function()
  request = {}
  local controller = { action_name = nil }
  local form = FormHelper.new{ controller = {controller_name = 'form'} }
  assert( form:url({}) == "/form/index", form:url({controller = "index"}) )
end

test.should_return_index_if_ommitted = function()
  request = {}
  local controller = { action_name = nil }
  local form = FormHelper.new{ controller = controller }
  assert( form:url({controller = "redirect"}) == "/redirect/index", form:url({controller = "index"}) )
end

test.should_return_action = function()
  request = {}
  local controller = { action_name = 'save' }
  local form = FormHelper.new{ controller = controller }
  assert( form:url({controller = "redirect"}) == "/redirect/save", form:url({controller = "redirect"}) )
end

test.should_return_action = function()
  request    = {}
  dispatcher = require('charon.dispatcher')
  dispatcher.prefix = 'app'
  local controller  = { action_name = 'save' }
  local form        = FormHelper.new{ controller = controller }
  local result      = form:url( {controller = "redirect"} )
  dispatcher.prefix = nil
  assert( result == '/app/redirect/save', result )
end

test.should_return_url_with_params = function()
  request = {}
  local controller = { action_name = 'save' }
  local form   = FormHelper.new{ controller = controller }
  local result = form:url({controller = "redirect", name = "Alex", id = 1})
  assert( result == "/redirect/save?id=1&name=Alex", result )
end

return test
