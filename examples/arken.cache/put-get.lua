-- arken.cache example: put / get
--
-- put()/get() NÃO passam por arken::json: o que você manda é exatamente o
-- que volta, byte a byte — sem quotes, sem escape, sem parse. São só uma
-- string entrando e uma string saindo. Por isso são binário-seguros de
-- verdade (aceitam byte nulo embutido no meio do valor, coisa que
-- insert/value não garantem hoje).
--
-- O custo dessa performance é a falta de conveniência: put/get só entendem
-- string. Se você precisar guardar uma tabela, faça o encode/decode você
-- mesmo (é o mesmo trabalho que insert/value já fazem por dentro — só que
-- você decide quando pagar esse custo, em vez de pagar sempre).
--
-- Números em examples/arken.cache/BENCHMARK.txt: put/get é bem mais rápido
-- que insert/value pra esse tipo de valor. Use quando: o dado já é string
-- (HTML cacheado, tokens, JSON que você mesmo serializou), ou quando
-- performance importa de verdade (alto volume de leitura/escrita).

local cache = require('arken.cache')
local json  = require('arken.json')

-- string simples: sem nenhuma serialização por baixo
cache.put('example:token', 'abc123', 60) -- expira em 60s
print('token: ' .. cache.get('example:token'))

-- fragmento de HTML pré-renderizado — exatamente o caso de uso que motivou
-- ter um bucket próprio pra isso (ver examples de arken.cache/bucket, se
-- existir, ou a documentação do módulo)
cache.put('example:fragment', '<div class="price">R$ 199,90</div>')
print('fragment: ' .. cache.get('example:fragment'))

-- se precisar de estrutura, o encode/decode fica explícito e é opcional —
-- só paga esse custo quem realmente precisa dele
cache.put('example:user-json', json.encode({ id = 42, name = 'Ada' }))
local user = json.decode(cache.get('example:user-json'))
print(string.format('user (via json manual): id=%d name=%s', user.id, user.name))

-- chave inexistente/expirada devolve nil, igual value()
print('missing: ' .. tostring(cache.get('example:does-not-exist')))

cache.remove('example:token')
cache.remove('example:fragment')
cache.remove('example:user-json')
