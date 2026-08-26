local cache = require('arken.cache')

local test = {}

local keys = {
  'tests-cache-remove-existing',
}

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should make value return nil after removing an existing key'] = function()
  cache.insert('tests-cache-remove-existing', 'value')
  assert( cache.value('tests-cache-remove-existing') == 'value' )
  cache.remove('tests-cache-remove-existing')
  assert( cache.value('tests-cache-remove-existing') == nil )
end

test['should not raise when removing a key that does not exist'] = function()
  assert( pcall(cache.remove, 'tests-cache-remove-does-not-exist') == true )
end

return test
