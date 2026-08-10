local cache = require('arken.cache')

local test = {}

local keys = {
  'tests-cache-get-expired',
}

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should return nil for a key that was never put'] = function()
  assert( cache.get('tests-cache-get-missing') == nil )
end

test['should return nil once the ttl has elapsed'] = function()
  cache.put('tests-cache-get-expired', 'short-lived', 1)
  assert( cache.get('tests-cache-get-expired') == 'short-lived' )
  os.sleep(2)
  assert( cache.get('tests-cache-get-expired') == nil )
end

return test
