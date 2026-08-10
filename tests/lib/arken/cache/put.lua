local cache = require('arken.cache')

local test = {}

local keys = {
  'tests-cache-put-plain',
  'tests-cache-put-overwrite',
  'tests-cache-put-null',
  'tests-cache-put-not-json',
}

test.after = function()
  for _, key in ipairs(keys) do
    cache.remove(key)
  end
end

test['should store a raw string readable back through get()'] = function()
  cache.put('tests-cache-put-plain', 'hello world')
  assert( cache.get('tests-cache-put-plain') == 'hello world' )
end

test['should overwrite the value of an existing key'] = function()
  cache.put('tests-cache-put-overwrite', 'first')
  cache.put('tests-cache-put-overwrite', 'second')
  assert( cache.get('tests-cache-put-overwrite') == 'second' )
end

test['should preserve an embedded null byte (binary-safe, unlike insert)'] = function()
  local withNull = 'abc\0def'
  assert( #withNull == 7 )

  cache.put('tests-cache-put-null', withNull)
  local back = cache.get('tests-cache-put-null')

  assert( #back == 7, #back )
  assert( back == withNull )
end

test['should not JSON-encode the value, unlike insert'] = function()
  local raw = '{"a":1}'
  cache.put('tests-cache-put-not-json', raw)
  assert( cache.get('tests-cache-put-not-json') == raw )
end

return test
