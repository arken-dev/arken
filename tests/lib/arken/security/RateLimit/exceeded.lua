local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should return false while under the limit'] = function()
  local rl = RateLimit.new('rl-test-count-under-limit', 3, 1)
  assert( rl.exceeded('10.0.0.1') == false )
  assert( rl.exceeded('10.0.0.1') == false )
end

test['should return true once the limit is reached'] = function()
  local rl = RateLimit.new('rl-test-count-trips-limit', 3, 1)
  rl.exceeded('10.0.0.2')
  rl.exceeded('10.0.0.2')
  assert( rl.exceeded('10.0.0.2') == true )
end

test['should keep returning true after the limit stays exceeded'] = function()
  local rl = RateLimit.new('rl-test-count-stays-tripped', 2, 1)
  rl.exceeded('10.0.0.3')
  assert( rl.exceeded('10.0.0.3') == true )
  assert( rl.exceeded('10.0.0.3') == true )
end

test['should count each ip independently'] = function()
  local rl = RateLimit.new('rl-test-count-per-ip', 1, 1)
  assert( rl.exceeded('10.0.0.4') == true )
  assert( rl.exceeded('10.0.0.5') == true )
  assert( rl.exceeded('10.0.0.4') == true, 'still over the limit for its own window' )
end

test['should reopen the window once it expires'] = function()
  local rl = RateLimit.new('rl-test-count-reopens-window', 2, 1)
  -- gc(0) simula "toda a janela já passou" sem esperar 60s reais: apaga o
  -- registro do IP, o que tem o mesmo efeito observável de uma nova janela
  -- (o próximo exceeded() começa do zero).
  rl.exceeded('10.0.0.6')
  assert( rl.exceeded('10.0.0.6') == true, 'sanity: 2nd hit trips the limit of 2' )

  rl.gc(0)

  assert( rl.exceeded('10.0.0.6') == false, 'a fresh window must not start already over the limit' )
end

test['should ignore an empty ip and return false'] = function()
  local rl = RateLimit.new('rl-test-count-empty-ip', 1, 1)
  assert( rl.exceeded('') == false )
end

return test
