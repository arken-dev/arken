local cache = require('arken.cache')

local test = {}

local bucketName = 'tests-cache-buckets-marker'
local bucket      = cache.bucket(bucketName)

test.after = function()
  for _, k in ipairs(bucket.keys('*')) do
    bucket.remove(k)
  end
  cache.remove('tests-cache-buckets-default-marker')
end

test['should include the default bucket once it has been used'] = function()
  cache.insert('tests-cache-buckets-default-marker', 'x')

  local names = {}
  for _, n in ipairs(cache.buckets()) do names[n] = true end
  assert( names['default'] )
end

test['should include a named bucket once cache.bucket() creates it'] = function()
  bucket.insert('marker', 'x')

  local names = {}
  for _, n in ipairs(cache.buckets()) do names[n] = true end
  assert( names[bucketName] )
end

return test
