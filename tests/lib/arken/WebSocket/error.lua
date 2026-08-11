local WebSocket = require 'arken.WebSocket'
local test = {}

test.should_be_noop_by_default = function()
  local ok = pcall(WebSocket.error, WebSocket, "timeout")
  assert(ok, "default error() deveria ser no-op")
end

return test
