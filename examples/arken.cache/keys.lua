-- arken.cache example: keys (busca por padrão glob)
--
-- cache.keys([pattern]) lista as chaves vivas (não expiradas) que casam com
-- um padrão glob — "*" (qualquer sequência), "?" (um caractere), "[...]"/
-- "[^...]" (classe de caracteres), igual os.glob(). Sem argumento, o padrão
-- é "*" (todas as chaves do bucket).

local cache = require('arken.cache')

cache.insert('session:abc123', 'a', 0)
cache.insert('session:def456', 'b', 0)
cache.insert('layout:home', 'c', 0)
cache.insert('layout:about', 'd', 0)

print('-- session:* --')
for _, key in ipairs(cache.keys('session:*')) do
  print('  ' .. key)
end

print('-- layout:* --')
for _, key in ipairs(cache.keys('layout:*')) do
  print('  ' .. key)
end

-- classe de caracteres: "layout:h*" ou "layout:a*" via [ha]
print('-- layout:[ha]* --')
for _, key in ipairs(cache.keys('layout:[ha]*')) do
  print('  ' .. key)
end

-- sem padrão, lista tudo que está no bucket
print('-- todas (sem padrão) --')
for _, key in ipairs(cache.keys()) do
  print('  ' .. key)
end

-- padrão sem match nenhum devolve tabela vazia
print('-- nope:* (sem match) --')
print('  quantidade: ' .. #cache.keys('nope:*'))

cache.remove('session:abc123')
cache.remove('session:def456')
cache.remove('layout:home')
cache.remove('layout:about')
