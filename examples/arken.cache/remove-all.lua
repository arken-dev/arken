-- arken.cache example: removeAll (remoção em massa por padrão glob)
--
-- cache.remove(key) só apaga uma chave exata. Pra apagar várias de uma vez
-- (ex.: invalidar tudo de um produto), use cache.removeAll(pattern) — mesmo
-- glob de keys() ("*", "?", "[...]"/"[^...]"). Diferente de keys(), o padrão
-- é obrigatório: não existe default "*" que apagaria o bucket inteiro por
-- um argumento esquecido.

local cache = require('arken.cache')

cache.insert('produto:1343', 'a', 0)
cache.insert('produto:1343:preco', 'b', 0)
cache.insert('produto:1343:estoque', 'c', 0)
cache.insert('produto:9999', 'd', 0)

print('-- antes --')
for _, key in ipairs(cache.keys('produto:*')) do
  print('  ' .. key)
end

-- equivalente a: for _, key in ipairs(cache.keys('produto:1343*')) do
--                  cache.remove(key)
--                end
-- mas numa única passada, sob um único lock do bucket.
cache.removeAll('produto:1343*')

print('-- depois --')
for _, key in ipairs(cache.keys('produto:*')) do
  print('  ' .. key)
end

-- padrão sem match nenhum não faz nada (nem dá erro)
cache.removeAll('produto:nao-existe:*')

-- chamar sem padrão é erro (diferente de keys(), que aceita chamada vazia)
print('-- removeAll() sem argumento --')
print('  ok? ' .. tostring(pcall(cache.removeAll)))

cache.remove('produto:9999')
