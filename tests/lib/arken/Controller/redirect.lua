local Controller = require('arken.Controller')
local test       = {}
local env        = {}

env.field = function(self, value)
  if value == "Host" then
    return 'localhost'
  end
  if value == "X-Forwarded-Proto" then
    return 'http'
  end
end

test.should_return_http_status_headers_and_body_nil = function()
  local c = Controller.new{ _env = env, controllerName = 'index' }
  local http, headers, body = c:redirect{ action = 'list' }
  assert( http == 302 )
  assert( headers[1] == "Location: http://localhost/index/list", headers[1] )
  assert( body == "" )
end

return test
