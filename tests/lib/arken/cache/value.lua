local cache = require('arken.cache')

local test = {}

local keys = {
  'tests-cache-value-missing',
  'tests-cache-value-expired',
}

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should return nil for a key that was never inserted'] = function()
  assert( cache.value('tests-cache-value-missing') == nil )
end

test['should return nil once the ttl has elapsed'] = function()
  cache.insert('tests-cache-value-expired', 'short-lived', 1)
  assert( cache.value('tests-cache-value-expired') == 'short-lived' )
  os.sleep(2)
  assert( cache.value('tests-cache-value-expired') == nil )
end

return test
