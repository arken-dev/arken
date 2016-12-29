local Controller = require 'charon.Controller'
local test = {}

test.should_return_http_status_500_if_errors = function()
  local c = Controller.new()
  c.myAction = function()
    error('myAction error...')
  end
  local status, headers, body = c:pexecute('myAction', {})
  assert( status == 500 )
end

test.should_return_empty_headers_if_errors = function()
  local c = Controller.new()
  c.myAction = function()
    error('myAction error...')
  end
  local status, headers, body = c:pexecute('myAction', {})
  assert( headers[1] == nil )
end

test.should_return_body_self_if_errors_is_a_string = function()
  local c = Controller.new()
  c.myAction = function()
    error({'myAction error...'})
  end
  local status, headers, body = c:pexecute('myAction', {})
  assert( body == 'myAction error...\n', body )
end

test.shold_3_returns_if_execute_without_error = function()
  local c = Controller.new()
  c.myAction = function()
    return 200, {}, "Hello World..."
  end
  local status, headers, body = c:pexecute('myAction', {})
  assert( status == 200 )
  assert( type(headers) == 'table' )
  assert( body == "Hello World...", body )
end

return test
