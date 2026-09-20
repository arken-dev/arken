local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should create an instance with limit and seconds'] = function()
  local rl = RateLimit.new(30, 60)
  assert( type(rl) == 'userdata', type(rl) )
end

test['should default seconds to 60 when omitted'] = function()
  local rl = RateLimit.new(1)
  assert( rl:count('10.0.0.1') == true )
end

return test
