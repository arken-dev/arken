-- arken.security.RateLimit example: basic usage
--
-- Janela fixa: new(name, limit, seconds) conta hits por IP dentro de uma
-- janela de `seconds` segundos (default: 60). count(ip) devolve true
-- quando o IP estoura o limite dentro da janela atual -- esse `true` é o
-- sinal para avisar a borda (Cloudflare, WAF, etc), não um erro.
--
-- `name` identifica um registro estático em C++ (fora de qualquer
-- lua_State): duas VMs do pool mvm chamando new() com o mesmo nome
-- enxergam o mesmo contador -- por isso a chamada é `limiter.count(ip)`,
-- ponto e não dois-pontos (mesmo esquema de arken.cache.bucket(name)).
-- new() com um nome já existente ignora limit/seconds e devolve a
-- instância já configurada pela primeira chamada.

local RateLimit = require('arken.security.RateLimit')

local limiter = RateLimit.new('example-basic-usage', 3, 60) -- 3 hits / 60 segundos
local ip      = '203.0.113.10'

for i = 1, 4 do
  local blocked = limiter.count(ip)
  print(string.format('hit %d -> count() = %s', i, tostring(blocked)))
end

-- login OK (ou qualquer evento que "perdoa" o IP): limpa o contador
limiter.clear(ip)
print('after clear -> count() = ' .. tostring(limiter.count(ip)))

-- opcional: count() já é varrido sozinho por uma thread de faxina
-- compartilhada (arken::security::RateLimit::backgroundGC), mas gc()
-- continua disponível pra forçar uma limpeza com outro critério
limiter.gc(3600)

print('size -> ' .. limiter.size() .. ' ip(s) rastreado(s)')

print('arken.security.RateLimit basic-usage OK')
