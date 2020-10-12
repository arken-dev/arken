local dispatcher = require('arken.dispatcher')
local test = {}

test.should_return_response = function()
  local status, headers, body = dispatcher.dispatchLocal('images/PIA00317.jpg')
  assert( status == 200, status )
  assert( headers[1] == 'Content-type: image/jpeg', headers[1] )
  assert( #body == 2059595, #body )
end

return test
