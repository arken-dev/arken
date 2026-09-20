local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should grow as distinct ips are counted'] = function()
  local rl = RateLimit.new(10, 60)
  rl:count('10.0.3.1')
  rl:count('10.0.3.2')
  rl:count('10.0.3.3')
  assert( rl:size() == 3, rl:size() )
end

test['should shrink after an explicit gc() removes idle ips'] = function()
  local rl = RateLimit.new(10, 1)
  rl:count('10.0.3.4')
  os.sleep(2)
  rl:gc(1)
  assert( rl:size() == 0, rl:size() )
end

test['should not grow without bound: idle ips are purged automatically, without any explicit gc() call'] = function()
  -- window de 1s -> a faxina amortizada dentro de count() dispara a cada
  -- 2s (2*window). Sem essa faxina, m_map só cresce: um IP visto uma
  -- única vez (ex.: um dos milhares de acessos distintos de um dia)
  -- nunca seria removido, porque count() só reseta a janela de um IP que
  -- volta a aparecer -- não apaga quem não volta.
  local rl = RateLimit.new(1, 1)

  rl:count('10.0.3.5')
  assert( rl:size() == 1 )

  os.sleep(2)

  -- nenhuma chamada a rl:gc() aqui: contar um IP totalmente novo é o
  -- suficiente pra disparar a faxina amortizada, que descarta 10.0.3.5
  -- (ocioso há mais de 2*window) antes de inserir o novo IP.
  rl:count('10.0.3.6')

  assert( rl:size() == 1, rl:size() )
end

return test
