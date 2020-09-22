local Controller = require('arken.Controller')
local test       = {}
local env        = {}

env.field = function()
  return 'localhost'
end

test.should_return_http_status_headers_and_body_nil = function()
  local c = Controller.new{ _env = env, controllerName = 'index' }
  local http, headers, body = c:redirect{ action = 'list' }
  assert( http == 302 )
  assert( headers[1] == "Location: http://localhost/index/list", headers[1] )
  assert( body == nil )
end

return test
