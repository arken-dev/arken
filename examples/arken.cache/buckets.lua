-- arken.cache example: bucket / buckets (múltiplos caches nomeados)
--
-- cache.bucket(nome) devolve um cache independente — próprio mutex, LRU e
-- maxSize — criado na primeira vez que esse nome é usado. Útil pra separar
-- usos com padrão de acesso e orçamento de memória diferentes (sessão vs.
-- fragmentos de HTML, por exemplo) sem que um interfira no outro.
--
-- cache.buckets() lista os nomes de todos os buckets já usados no processo
-- — serve pra introspecção/debug (ex.: um endpoint admin mostrando o que
-- está em uso).

local cache = require('arken.cache')

local session = cache.bucket('sessao')
local html    = cache.bucket('html')

session.insert('user:1', { id = 1, nome = 'Ada' })
html.put('home:fragment', '<div>bem-vindo</div>')

-- listar todos os buckets já criados no processo
print('-- buckets em uso --')
for _, name in ipairs(cache.buckets()) do
  print('  ' .. name)
end

-- procurando um bucket específico pelo nome na lista
local function hasBucket(name)
  for _, n in ipairs(cache.buckets()) do
    if n == name then
      return true
    end
  end
  return false
end

print('tem bucket "sessao"? ' .. tostring(hasBucket('sessao')))
print('tem bucket "não-existe"? ' .. tostring(hasBucket('nao-existe')))

-- cache.bucket(nome) sempre devolve o MESMO bucket pra um nome repetido —
-- não precisa guardar a referência, dá pra pedir de novo em qualquer lugar
local sameSession = cache.bucket('sessao')
print('mesmo bucket? ' .. tostring(sameSession.value('user:1').nome == 'Ada'))

session.remove('user:1')
html.remove('home:fragment')
