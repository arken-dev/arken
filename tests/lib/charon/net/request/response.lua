local HttpParser = require "charon.net.HttpParser"
request = require('charon.net.request')
local test = {}

test.should_append_headers= function()
  request.cookie('box', 'full') 
  local headers = {}
  request.response(headers)
  assert(headers[1] == 'Set-Cookie: box=full', headers[1])
end

return test
