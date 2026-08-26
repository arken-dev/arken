local cache = require('arken.cache')
local json  = require('arken.json')

local test = {}

local key = 'tests-cache-size-key'

test.after = function()
  cache.remove(key)
end

test['should grow by the byte length of the encoded value after insert'] = function()
  local value  = { some = 'payload', n = 42 }
  local before = cache.size()
  cache.insert(key, value, 0)
  local after = cache.size()
  assert( after - before == #json.encode(value) )
end

test['should shrink back after removing the inserted key'] = function()
  cache.insert(key, 'x', 0)
  local before = cache.size()
  cache.remove(key)
  local after = cache.size()
  assert( before - after == #json.encode('x') )
end

return test
