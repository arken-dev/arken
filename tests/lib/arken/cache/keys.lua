local cache = require('arken.cache')

local test = {}

local keys = {
  'tests-cache-keys-session-abc',
  'tests-cache-keys-session-def',
  'tests-cache-keys-layout-home',
  'tests-cache-keys-expired',
}

local function toSet(list)
  local set = {}
  for _, v in ipairs(list) do
    set[v] = true
  end
  return set
end

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should return only the keys matching the glob pattern'] = function()
  cache.insert('tests-cache-keys-session-abc', 'a', 0)
  cache.insert('tests-cache-keys-session-def', 'b', 0)
  cache.insert('tests-cache-keys-layout-home', 'c', 0)

  local sessions = toSet(cache.keys('tests-cache-keys-session-*'))
  assert( sessions['tests-cache-keys-session-abc'] )
  assert( sessions['tests-cache-keys-session-def'] )
  assert( sessions['tests-cache-keys-layout-home'] == nil )
end

test['should default to matching every key when called without a pattern'] = function()
  cache.insert('tests-cache-keys-session-abc', 'a', 0)
  local all = toSet(cache.keys())
  assert( all['tests-cache-keys-session-abc'] )
end

test['should return an empty table when nothing matches'] = function()
  local result = cache.keys('tests-cache-keys-does-not-exist-*')
  assert( type(result) == 'table' )
  assert( #result == 0 )
end

test['should exclude an expired key even before gc runs'] = function()
  cache.insert('tests-cache-keys-expired', 'short-lived', 1)
  os.sleep(2)
  local all = toSet(cache.keys('tests-cache-keys-*'))
  assert( all['tests-cache-keys-expired'] == nil )
end

return test
