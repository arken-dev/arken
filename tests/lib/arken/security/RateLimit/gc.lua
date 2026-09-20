local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should drop an ip idle for longer than idle_seconds'] = function()
  local rl = RateLimit.new(2, 1)
  rl:count('10.0.2.1')
  assert( rl:count('10.0.2.1') == true, 'sanity: 2nd hit trips the limit of 2' )

  os.sleep(2)
  rl:gc(1) -- qualquer ip ocioso há mais de 1s é removido

  assert( rl:count('10.0.2.1') == false, 'record must have been purged, so this is a fresh first hit' )
end

test['should keep an ip that has not been idle long enough'] = function()
  local rl = RateLimit.new(2, 1)
  rl:count('10.0.2.2')
  assert( rl:count('10.0.2.2') == true, 'sanity: 2nd hit trips the limit of 2' )

  rl:gc(3600) -- ninguém fica ocioso por 1h num teste

  assert( rl:count('10.0.2.2') == true, 'record must survive, still over the limit' )
end

test['should not raise when called on an empty instance'] = function()
  local rl = RateLimit.new(1, 1)
  assert( pcall(rl.gc, rl, 3600) == true )
end

return test
