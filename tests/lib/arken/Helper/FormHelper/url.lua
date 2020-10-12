local FormHelper = require('arken.Helper.FormHelper')
local dispatcher = require('arken.dispatcher')
local test = {}

test.before = function()
  dispatcher.prefix = ""
end

test.should_return_url_string = function()
  local form = FormHelper.new()
  assert( form:url("/app/redirect") == "/app/redirect" )
end

test.should_return_action_and_controller_ommitted = function()
  request = {}
  local controller = { actionName = nil }
  local form = FormHelper.new{ controller = {} }
  assert( form:url({}) == "/index/index", form:url({controller = "index"}) )
end

test.should_return_action_and_controller_ommitted_and_inside_controller_form = function()
  request = {}
  local controller = { actionName = nil }
  local form = FormHelper.new{ controller = {controllerName = 'form'} }
  assert( form:url({}) == "/form/index", form:url({controller = "index"}) )
end

test.should_return_index_if_ommitted = function()
  request = {}
  local controller = { actionName = nil }
  local form = FormHelper.new{ controller = controller }
  assert( form:url({controller = "redirect"}) == "/redirect/index", form:url({controller = "index"}) )
end

test.should_return_action = function()
  request = {}
  local controller = { actionName = 'save' }
  local form = FormHelper.new{ controller = controller }
  assert( form:url({controller = "redirect"}) == "/redirect/save", form:url({controller = "redirect"}) )
end

test.should_return_action = function()
  request    = {}
  dispatcher.prefix = '/app'
  local controller  = { actionName = 'save' }
  local form        = FormHelper.new{ controller = controller }
  local result      = form:url( {controller = "redirect"} )
  dispatcher.prefix = ''
  assert( result == '/app/redirect/save', result )
end

test.should_return_url_with_params = function()
  request = {}
  local controller = { actionName = 'save' }
  local form   = FormHelper.new{ controller = controller }
  local result = form:url({controller = "redirect", name = "Alex", id = 1})
  assert( result == "/redirect/save?id=1&name=Alex", result )
end

return test
