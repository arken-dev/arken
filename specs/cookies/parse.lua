local cookie =require 'oberon.cookie'
local should = require "test.should"
local test   = {}

test['should return __gads'] = function()
  local buffer = os.read(OBERON_PATH .. "/specs/cookies/1-example.cookie")
  local result = cookie.parse(buffer)
  should.be_equal(result.__gads, "ID=65db397af496b42f:T=1459784587:S=ALNI_MaXlE55QEou3h9txkA8jA5kPzMKVg")
end

test['return _session_id'] = function()
  local buffer = os.read(OBERON_PATH .. "/specs/cookies/1-example.cookie")
  local result = cookie.parse(buffer)
  should.be_equal(result._session_id, "5c08fc52513b2d562c62a1f7252dc834")
end

return test
