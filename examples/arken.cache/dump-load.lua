-- arken.cache example: dump / load (persistência em disco)
--
-- bucket:dump(caminho) grava as chaves de um bucket num arquivo binário
-- próprio; bucket:load(caminho) restaura de lá, preservando o TTL restante
-- de cada chave (uma chave com 3h pra expirar continua com ~3h depois do
-- load) — chaves já expiradas no momento do load() são descartadas, não
-- restauradas.
--
-- Serve pro caso raro de restart de verdade do processo (ex.: deploy de
-- ajuste em C++) — o hot reload do dia a dia (Lua) nunca derruba o
-- processo, então o cache já sobrevive a ele sozinho, sem precisar de
-- dump/load pra isso.

local cache = require('arken.cache')
local mvm   = require('arken.mvm')

local session = cache.bucket('dump-load-example')
local path    = mvm.path() .. '/tmp/tmp/example-cache-dump.bin'

session.put('user:1', 'ainda aqui depois do restart', 3600) -- 1h
session.put('user:2', 'nunca expira', 0)
session.put('user:3', 'vai expirar antes do load', 1)

print('dump: ' .. tostring(session.dump(path)))

-- espera 'user:3' expirar de verdade antes do load, só pra mostrar que
-- chave já expirada não volta
os.sleep(2)

-- simula o processo "caindo e subindo de novo": limpa tudo da memória
for _, key in ipairs(session.keys('*')) do
  session.remove(key)
end
print('bucket vazio antes do load? ' .. tostring(#session.keys('*') == 0))

print('load: ' .. tostring(session.load(path)))

print('user:1 -> ' .. tostring(session.get('user:1')))
print('user:2 -> ' .. tostring(session.get('user:2')))
print('user:3 -> ' .. tostring(session.get('user:3')) .. ' (esperado: nil, já tinha expirado)')

-- load() num arquivo que não existe não dá erro, só devolve false
print('load em arquivo inexistente: ' .. tostring(session.load('/tmp/nao-existe.bin')))

session.remove('user:1')
session.remove('user:2')
os.remove(path)
