local WebSocket = require 'arken.WebSocket'
local test = {}

test.should_be_noop_by_default = function()
  local ok = pcall(WebSocket.onError, WebSocket, "timeout")
  assert(ok, "default onError() deveria ser no-op")
end

return test
