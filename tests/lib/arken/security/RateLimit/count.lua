local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should return false while under the limit'] = function()
  local rl = RateLimit.new(3, 1)
  assert( rl:count('10.0.0.1') == false )
  assert( rl:count('10.0.0.1') == false )
end

test['should return true once the limit is reached'] = function()
  local rl = RateLimit.new(3, 1)
  rl:count('10.0.0.2')
  rl:count('10.0.0.2')
  assert( rl:count('10.0.0.2') == true )
end

test['should keep returning true after the limit stays exceeded'] = function()
  local rl = RateLimit.new(2, 1)
  rl:count('10.0.0.3')
  assert( rl:count('10.0.0.3') == true )
  assert( rl:count('10.0.0.3') == true )
end

test['should count each ip independently'] = function()
  local rl = RateLimit.new(1, 1)
  assert( rl:count('10.0.0.4') == true )
  assert( rl:count('10.0.0.5') == true )
  assert( rl:count('10.0.0.4') == true, 'still over the limit for its own window' )
end

test['should reopen the window once it expires'] = function()
  local rl = RateLimit.new(2, 1)
  -- gc(0) simula "toda a janela já passou" sem esperar 60s reais: apaga o
  -- registro do IP, o que tem o mesmo efeito observável de uma nova janela
  -- (o próximo count() começa do zero).
  rl:count('10.0.0.6')
  assert( rl:count('10.0.0.6') == true, 'sanity: 2nd hit trips the limit of 2' )

  rl:gc(0)

  assert( rl:count('10.0.0.6') == false, 'a fresh window must not start already over the limit' )
end

test['should ignore an empty ip and return false'] = function()
  local rl = RateLimit.new(1, 1)
  assert( rl:count('') == false )
end

return test
