local cookie = require 'charon.net.cookie'
local test   = {}

test['should return __gads'] = function()
  local buffer = os.read(CHARON_PATH .. "/tests/lib/charon/net/cookie/1-example.cookie")
  local result = cookie.parse(buffer)
  assert(result.__gads == "ID=65db397af496b42f:T=1459784587:S=ALNI_MaXlE55QEou3h9txkA8jA5kPzMKVg")
end

test['return _session_id'] = function()
  local buffer = os.read(CHARON_PATH .. "/tests/lib/charon/net/cookie/1-example.cookie")
  local result = cookie.parse(buffer)
  assert(result._session_id == "5c08fc52513b2d562c62a1f7252dc834")
end

test['return one value _session_id'] = function()
  local buffer = os.read(CHARON_PATH .. "/tests/lib/charon/net/cookie/2-example.cookie")
  local result = cookie.parse(buffer)
  assert(result._session_id == "fddb48d0a5fed89d837633ce6652a584")
end

return test
