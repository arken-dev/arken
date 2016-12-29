local Controller = require('charon.Controller')
local test       = {}

test.should_return_http_status_headers_and_body_nil = function()
  request = { field = function() return 'localhost' end }
  local c = Controller.new{ controller_name = 'index' }
  local http, headers, body = c:redirect{ action = 'list' }
  assert( http == 302 )
  assert( headers[1] == "Location: http://localhost/index/list", headers[1] )
  assert( body == nil )
end

return test
