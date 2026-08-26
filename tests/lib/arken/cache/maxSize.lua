local cache = require('arken.cache')
local json  = require('arken.json')

local test = {}

local prefix = 'tests-cache-maxsize-'
local unit   = #json.encode('x') -- bytes de um valor codificado de 1 caractere

local function toSet(list)
  local set = {}
  for _, v in ipairs(list) do
    set[v] = true
  end
  return set
end

-- maxSize é global ao processo (assim como o resto do cache), então cada
-- teste precisa deixar tudo como encontrou: sem limite, sem chaves suas.
test.after = function()
  for _, key in ipairs(cache.keys(prefix .. '*')) do
    cache.remove(key)
  end
  cache.maxSize(0)
end

test['should have no limit by default'] = function()
  assert( cache.maxSize() == 0 )
end

test['should evict the least recently used key once the cap is exceeded'] = function()
  cache.maxSize(unit * 3)

  cache.insert(prefix .. 'a', 'x', 0)
  cache.insert(prefix .. 'b', 'x', 0)
  cache.insert(prefix .. 'c', 'x', 0)

  -- as 3 cabem exatamente no limite
  local present = toSet(cache.keys(prefix .. '*'))
  assert( present[prefix .. 'a'] and present[prefix .. 'b'] and present[prefix .. 'c'] )

  -- 4a chave estoura o limite: precisa despejar a menos recentemente usada (a)
  cache.insert(prefix .. 'd', 'x', 0)

  present = toSet(cache.keys(prefix .. '*'))
  assert( present[prefix .. 'a'] == nil, 'least recently used key should have been evicted' )
  assert( present[prefix .. 'b'] )
  assert( present[prefix .. 'c'] )
  assert( present[prefix .. 'd'] )
end

test['should protect a key from eviction by reading it before the cap is hit'] = function()
  cache.maxSize(unit * 3)

  cache.insert(prefix .. 'a', 'x', 0)
  cache.insert(prefix .. 'b', 'x', 0)
  cache.insert(prefix .. 'c', 'x', 0)

  -- toca 'a' (agora é a mais recentemente usada); 'b' vira a candidata a LRU
  cache.value(prefix .. 'a')

  cache.insert(prefix .. 'd', 'x', 0)

  local present = toSet(cache.keys(prefix .. '*'))
  assert( present[prefix .. 'a'], 'key touched via value() should survive eviction' )
  assert( present[prefix .. 'b'] == nil, 'untouched key should be the one evicted' )
end

test['should never let size() exceed maxSize() after inserts'] = function()
  cache.maxSize(unit * 2)

  for i = 1, 10 do
    cache.insert(prefix .. 'n' .. i, 'x', 0)
  end

  assert( cache.size() <= cache.maxSize() )
end

test['should evict immediately when the cap is lowered, without waiting for an insert'] = function()
  cache.maxSize(unit * 5)
  for i = 1, 5 do
    cache.insert(prefix .. 'n' .. i, 'x', 0)
  end
  assert( #cache.keys(prefix .. '*') == 5 )

  cache.maxSize(unit)

  assert( #cache.keys(prefix .. '*') == 1 )
end

test['should remove the cap when set back to 0'] = function()
  cache.maxSize(unit)
  cache.insert(prefix .. 'only', 'x', 0)

  cache.maxSize(0)
  for i = 1, 10 do
    cache.insert(prefix .. 'n' .. i, 'x', 0)
  end

  assert( #cache.keys(prefix .. '*') == 11 )
end

return test
