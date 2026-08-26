local cache = require('arken.cache')
local json  = require('arken.json')

local test = {}

local expiredKey    = 'tests-cache-gc-expired'
local persistentKey = 'tests-cache-gc-persistent'

test.after = function()
  cache.remove(expiredKey)
  cache.remove(persistentKey)
end

test['should purge expired entries and keep non-expired ones'] = function()
  cache.insert(expiredKey, 'gone-soon', 1)
  cache.insert(persistentKey, 'stays', 0)
  os.sleep(2)

  -- size() is checked before any value() call on expiredKey, since value()
  -- also purges expired entries as a side effect (lazy expiration) and would
  -- make it impossible to tell gc() apart from that.
  local before = cache.size()
  cache.gc()
  local after = cache.size()

  assert( before - after == #json.encode('gone-soon') )
  assert( cache.value(persistentKey) == 'stays' )
end

return test
