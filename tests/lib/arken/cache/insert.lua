local cache = require('arken.cache')
local json  = require('arken.json')

local test = {}

local keys = {
  'tests-cache-insert-string',
  'tests-cache-insert-table',
  'tests-cache-insert-overwrite',
  'tests-cache-insert-never-expires',
  'tests-cache-insert-background-gc',
}

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should store and read back a string value'] = function()
  cache.insert('tests-cache-insert-string', 'hello')
  assert( cache.value('tests-cache-insert-string') == 'hello' )
end

test['should store and read back a table value'] = function()
  cache.insert('tests-cache-insert-table', { a = 1, b = 'x' })
  local result = cache.value('tests-cache-insert-table')
  assert( type(result) == 'table' )
  assert( result.a == 1 )
  assert( result.b == 'x' )
end

test['should overwrite the value of an existing key'] = function()
  cache.insert('tests-cache-insert-overwrite', 'first')
  cache.insert('tests-cache-insert-overwrite', 'second')
  assert( cache.value('tests-cache-insert-overwrite') == 'second' )
end

test['should never expire when expires is 0'] = function()
  cache.insert('tests-cache-insert-never-expires', 'persisted', 0)
  os.sleep(2)
  assert( cache.value('tests-cache-insert-never-expires') == 'persisted' )
end

test['should purge an expired key on its own, without any explicit gc() call'] = function()
  local key = 'tests-cache-insert-background-gc'
  cache.insert(key, 'short-lived', 1)
  local before = cache.size()
  -- size() é medido antes/depois em vez de ler a chave com cache.value(),
  -- porque value() também purga entradas expiradas como efeito colateral
  -- (expiração lazy) e mascararia se foi a thread de background que agiu.
  -- nenhuma chamada a cache.gc() aqui: o cache tem uma thread própria que
  -- varre e libera entradas expiradas periodicamente, então isso precisa
  -- desaparecer sozinho depois de um tempo maior que o intervalo dela.
  os.sleep(7)
  local after = cache.size()
  assert( before - after == #json.encode('short-lived'), string.format('before=%d after=%d', before, after) )
end

return test
