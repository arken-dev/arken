local RateLimit = require('arken.security.RateLimit')

local test = {}

test['should grow as distinct ips are counted'] = function()
  local rl = RateLimit.new('rl-test-size-grows', 10, 60)
  rl.count('10.0.3.1')
  rl.count('10.0.3.2')
  rl.count('10.0.3.3')
  assert( rl.size() == 3, rl.size() )
end

test['should shrink after an explicit gc() removes idle ips'] = function()
  local rl = RateLimit.new('rl-test-size-explicit-gc', 10, 1)
  rl.count('10.0.3.4')
  os.sleep(2)
  rl.gc(1)
  assert( rl.size() == 0, rl.size() )
end

test['should not grow without bound: idle ips are purged automatically, without any explicit gc() call'] = function()
  -- window de 1s -> a instância registrada sob esse nome é varrida pela
  -- thread de faxina compartilhada (arken::security::RateLimit::backgroundGC,
  -- mesmo esquema do arken::cache), que roda a cada alguns segundos e
  -- descarta IPs ociosos há mais de 2x a janela. Sem essa thread, m_map só
  -- cresce: um IP visto uma única vez (ex.: um dos milhares de acessos
  -- distintos de um dia) nunca seria removido, porque count() só reseta
  -- a janela de um IP que volta a aparecer -- não apaga quem não volta.
  local rl = RateLimit.new('rl-test-size-background-gc', 1, 1)

  rl.count('10.0.3.5')
  assert( rl.size() == 1 )

  -- nenhuma chamada a rl.gc() aqui: só espera a thread de background
  -- (roda a cada 5s) encontrar o ip ocioso (> 2x window = 2s) sozinha.
  os.sleep(7)

  assert( rl.size() == 0, rl.size() )
end

return test
