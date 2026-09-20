local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should reset the hit counter for that ip'] = function()
  local rl = RateLimit.new('rl-test-clear-resets', 2, 1)
  assert( rl.exceeded('10.0.1.1') == false )
  assert( rl.exceeded('10.0.1.1') == true )

  rl.clear('10.0.1.1')

  assert( rl.exceeded('10.0.1.1') == false, 'first hit after clear must not be over the limit' )
end

test['should not affect other ips'] = function()
  local rl = RateLimit.new('rl-test-clear-isolated', 1, 1)
  rl.exceeded('10.0.1.2')
  rl.exceeded('10.0.1.3')

  rl.clear('10.0.1.2')

  assert( rl.exceeded('10.0.1.2') == true )
  assert( rl.exceeded('10.0.1.3') == true, 'still over its own limit, untouched by clearing 10.0.1.2' )
end

test['should not raise when clearing an ip that was never counted'] = function()
  local rl = RateLimit.new('rl-test-clear-unknown-ip', 1, 1)
  assert( pcall(rl.clear, '10.0.1.4') == true )
end

return test
