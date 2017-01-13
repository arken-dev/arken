local dispatcher = require('charon.dispatcher')
local test = {}

test.should_return_response = function()
  local status, headers, body = dispatcher.test()
  assert( status == 200, status )
  assert( headers[1] == 'Content-Type: text/html; charset=utf-8', headers[1] )
  assert( body == '<html><body><h1>DISPATCHER TEST IS WORKS ...</h1></body></html>', body )
end

return test
