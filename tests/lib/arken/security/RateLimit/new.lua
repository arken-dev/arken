local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should create a table of methods bound to a name'] = function()
  local rl = RateLimit.new('rl-test-new-1', 30, 60)
  assert( type(rl) == 'table', type(rl) )
  assert( type(rl.exceeded) == 'function' )
end

test['should default seconds to 60 when omitted'] = function()
  local rl = RateLimit.new('rl-test-new-2', 1)
  assert( rl.exceeded('10.0.0.1') == true )
end

test['should share state across separate new() calls with the same name'] = function()
  -- registro estático em C++ (RateLimit::get), não um objeto Lua local:
  -- duas chamadas com o mesmo nome apontam pra mesma instância -- é assim
  -- que o mesmo contador fica visível em qualquer VM do pool mvm que
  -- chame RateLimit.new() com esse nome.
  local a = RateLimit.new('rl-test-new-shared', 2, 60)
  local b = RateLimit.new('rl-test-new-shared', 2, 60)

  a.exceeded('10.0.0.9')

  assert( b.size() == 1, b.size() )
end

test['should ignore limit/seconds on calls after the name already exists'] = function()
  local a = RateLimit.new('rl-test-new-first-wins', 1, 60) -- limit=1: qualquer hit já estoura
  local b = RateLimit.new('rl-test-new-first-wins', 999, 999) -- ignorado: nome já existia

  assert( b.exceeded('10.0.0.10') == true, 'limit continua sendo o da primeira chamada (1), não 999' )
end

return test
