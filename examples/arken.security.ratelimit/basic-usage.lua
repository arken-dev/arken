-- arken.security.RateLimit example: basic usage
--
-- Janela fixa: new(limit, minutes) conta hits por IP dentro de uma janela
-- de `minutes` minutos. count(ip) devolve true quando o IP estoura o
-- limite dentro da janela atual -- esse `true` é o sinal para avisar a
-- borda (Cloudflare, WAF, etc), não um erro.

local RateLimit = require('arken.security.RateLimit')

local limiter = RateLimit.new(3, 1) -- 3 hits / 1 minuto
local ip      = '203.0.113.10'

for i = 1, 4 do
  local blocked = limiter:count(ip)
  print(string.format('hit %d -> count() = %s', i, tostring(blocked)))
end

-- login OK (ou qualquer evento que "perdoa" o IP): limpa o contador
limiter:clear(ip)
print('after clear -> count() = ' .. tostring(limiter:count(ip)))

-- chamado periodicamente (ex.: por um timer), não por request: libera
-- memória de IPs que não geram evento ruim há muito tempo
limiter:gc(3600)

print('arken.security.RateLimit basic-usage OK')
