local Controller = require 'arken.Controller'
local test = {}

test.should_return_http_status_200 = function()
  local c = Controller.new()
  local status, headers, body = c:render({ output = 'text', value = "Hello ..."})
  assert( status == 200 )
end

test.should_return_http_status_defined = function()
  local c = Controller.new()
  local status, headers, body = c:render({ code = 500, output = 'text', value = "Hello ..."})
  assert( status == 500 )
end

test.should_return_http_headers = function()
  local c = Controller.new()
  local status, headers, body = c:render({ code = 500, output = 'text', value = "Hello ..."})
  assert( headers[1] == 'Content-Type: text/plain' )
end

test.should_return_body_is_value = function()
  local c = Controller.new()
  local status, headers, body = c:render({ code = 500, output = 'text', value = "Hello ..."})
  assert( body == 'Hello ...' )
end

return test
