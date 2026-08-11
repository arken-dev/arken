local WebSocket = require 'arken.WebSocket'
local test = {}

test.should_accept_by_default_when_not_overridden = function()
  local status, headers, body = WebSocket:handshake({})
  assert(status == 200, status)
  assert(type(headers) == 'table', type(headers))
  assert(#headers == 0, #headers)
  assert(body == "", body)
end

return test
