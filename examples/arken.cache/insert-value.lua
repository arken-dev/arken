-- arken.cache example: insert / value
--
-- insert()/value() passam por arken::json por dentro: aceitam qualquer
-- valor Lua (tabela, número, boolean, string) e fazem o encode/decode
-- automaticamente. É o par prático — você guarda uma tabela igual guardaria
-- em qualquer variável Lua, sem se preocupar em serializar nada.
--
-- O custo dessa praticidade é medido em examples/arken.cache/BENCHMARK.txt:
-- o encode/decode de JSON responde por boa parte do tempo de cada operação
-- (mais na leitura que na escrita). Pra valores que já são string simples
-- (HTML cacheado, tokens, JSON pré-serializado por você mesmo), o par
-- put()/get() (ver put-get.lua nesta pasta) é bem mais rápido por pular
-- essa camada inteira.
--
-- Use insert/value quando: o valor é uma estrutura (tabela), a conveniência
-- de não serializar manualmente importa mais que performance bruta, ou o
-- volume de chamadas é baixo o suficiente pra a diferença não importar.

local cache = require('arken.cache')

-- tabela simples, tipo dado de sessão de usuário
cache.insert('example:user', { id = 42, name = 'Ada', admin = true })

local user = cache.value('example:user')
print(string.format('user: id=%d name=%s admin=%s', user.id, user.name, tostring(user.admin)))

-- tabela aninhada, listas dentro de tabela
cache.insert('example:cart', {
  items = { 'teclado', 'mouse', 'monitor' },
  total = 1234.56,
})

local cart = cache.value('example:cart')
print(string.format('cart total: %.2f, %d itens', cart.total, #cart.items))
for _, item in ipairs(cart.items) do
  print('  - ' .. item)
end

-- string simples também funciona (só que paga o mesmo custo de JSON)
cache.insert('example:token', 'abc123', 60) -- expira em 60s
print('token: ' .. cache.value('example:token'))

-- chave inexistente/expirada devolve nil
print('missing: ' .. tostring(cache.value('example:does-not-exist')))

cache.remove('example:user')
cache.remove('example:cart')
cache.remove('example:token')
