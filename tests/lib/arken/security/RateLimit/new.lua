local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should create an instance with limit and minutes'] = function()
  local rl = RateLimit.new(30, 1)
  assert( type(rl) == 'userdata', type(rl) )
end

test['should default minutes to 1 when omitted'] = function()
  local rl = RateLimit.new(1)
  assert( rl:count('10.0.0.1') == true )
end

return test
