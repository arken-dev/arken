local cache = require('arken.cache')
local json  = require('arken.json')

local test = {}

local sessionBucket = cache.bucket('tests-cache-bucket-sessao')
local htmlBucket     = cache.bucket('tests-cache-bucket-html')

test.after = function()
  for _, k in ipairs(sessionBucket.keys('*')) do sessionBucket.remove(k) end
  for _, k in ipairs(htmlBucket.keys('*')) do htmlBucket.remove(k) end
  sessionBucket.maxSize(0)
  htmlBucket.maxSize(0)
  cache.remove('tests-cache-bucket-default-key')
end

test['should isolate storage from the default bucket'] = function()
  cache.insert('tests-cache-bucket-default-key', 'default-value')
  sessionBucket.insert('tests-cache-bucket-default-key', 'session-value')

  assert( cache.value('tests-cache-bucket-default-key') == 'default-value' )
  assert( sessionBucket.value('tests-cache-bucket-default-key') == 'session-value' )
end

test['should isolate storage between two different named buckets'] = function()
  sessionBucket.insert('same-key', 'from-session')
  htmlBucket.insert('same-key', 'from-html')

  assert( sessionBucket.value('same-key') == 'from-session' )
  assert( htmlBucket.value('same-key') == 'from-html' )
end

test['should return the same underlying bucket across separate cache.bucket() calls'] = function()
  sessionBucket.insert('persisted', 'x', 0)

  local sameBucket = cache.bucket('tests-cache-bucket-sessao')
  assert( sameBucket.value('persisted') == 'x' )
end

test['should not let one bucket evict keys from another bucket under memory pressure'] = function()
  local unit = #json.encode('x')

  sessionBucket.maxSize(0)          -- sessão sem limite
  htmlBucket.maxSize(unit)          -- html só cabe 1 chave

  sessionBucket.insert('protected', 'x', 0)

  htmlBucket.insert('frag-a', 'x', 0)
  htmlBucket.insert('frag-b', 'x', 0) -- estoura o limite do bucket html

  assert( sessionBucket.value('protected') == 'x', 'session bucket must be untouched by html bucket eviction' )
  assert( #htmlBucket.keys('*') == 1, 'html bucket enforces its own cap independently' )
end

test['should exclude an expired key from keys() scoped to that bucket'] = function()
  sessionBucket.insert('expiring', 'x', 1)
  os.sleep(2)

  local present = {}
  for _, k in ipairs(sessionBucket.keys('*')) do present[k] = true end
  assert( present['expiring'] == nil )
end

test['should support put/get (raw, no json) scoped to a named bucket'] = function()
  sessionBucket.put('raw-key', 'raw-value')
  assert( sessionBucket.get('raw-key') == 'raw-value' )
  assert( htmlBucket.get('raw-key') == nil, 'put on one bucket must not leak into another' )
  sessionBucket.remove('raw-key')
end

test['should purge a named bucket automatically, without any explicit gc() call'] = function()
  sessionBucket.insert('auto-gc-me', 'x', 1)
  local before = sessionBucket.size()

  -- nenhuma chamada a sessionBucket.gc() aqui: a thread de background varre
  -- todos os buckets, não só o default.
  os.sleep(7)

  local after = sessionBucket.size()
  assert( after < before, string.format('before=%d after=%d', before, after) )
end

return test
