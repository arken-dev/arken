local Controller = require 'charon.Controller'
local test = {}

test.should_return_http_status_500 = function()
  local c = Controller.new()
  local status, headers, body = c:rescue("error...")
  assert( status == 500 )
end

test.should_return_empty_headers = function()
  local c = Controller.new()
  local status, headers, body = c:rescue("error...")
  assert( headers[1] == nil )
end

test.should_return_body_self_if_errors_is_a_string = function()
  local c = Controller.new()
  local status, headers, body = c:rescue("error...")
  assert( body == 'error...' )
end

test.should_return_body_concat_table = function()
  local c = Controller.new()
  local errors = {"Check the errors below", "name is required"}
  local status, headers, body = c:rescue(errors)
  assert( body == "Check the errors below\nname is required\n", body )
end

return test
