local cache = require('arken.cache')

local test = {}

local keys = {
  'tests-cache-removeAll-session-abc',
  'tests-cache-removeAll-session-def',
  'tests-cache-removeAll-layout-home',
}

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should remove only the keys matching the glob pattern'] = function()
  cache.insert('tests-cache-removeAll-session-abc', 'a', 0)
  cache.insert('tests-cache-removeAll-session-def', 'b', 0)
  cache.insert('tests-cache-removeAll-layout-home', 'c', 0)

  cache.removeAll('tests-cache-removeAll-session-*')

  assert( cache.value('tests-cache-removeAll-session-abc') == nil )
  assert( cache.value('tests-cache-removeAll-session-def') == nil )
  assert( cache.value('tests-cache-removeAll-layout-home') == 'c' )
end

test['should not raise when the pattern matches nothing'] = function()
  assert( pcall(cache.removeAll, 'tests-cache-removeAll-does-not-exist-*') == true )
end

test['should require an explicit pattern, unlike keys()'] = function()
  assert( pcall(cache.removeAll) == false )
end

test['should scope removal to a named bucket, leaving the default bucket untouched'] = function()
  local bucket = cache.bucket('tests-cache-removeAll-bucket')
  bucket.insert('session-abc', 'a', 0)
  cache.insert('tests-cache-removeAll-session-abc', 'a', 0)

  bucket.removeAll('session-*')

  assert( bucket.value('session-abc') == nil )
  assert( cache.value('tests-cache-removeAll-session-abc') == 'a' )
end

return test
