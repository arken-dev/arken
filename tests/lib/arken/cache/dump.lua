local cache = require('arken.cache')
local mvm   = require('arken.mvm')

local test = {}

local bucketName = 'tests-cache-dump'
local bucket      = cache.bucket(bucketName)
local path        = mvm.path() .. '/tmp/tmp/tests-cache-dump.bin'

test.after = function()
  for _, k in ipairs(bucket.keys('*')) do
    bucket.remove(k)
  end
  os.remove(path)
end

test['should return true and write a file that load() can read back'] = function()
  bucket.insert('k1', 'v1', 0)

  assert( bucket.dump(path) == true )
  assert( os.exists(path) )

  bucket.remove('k1')
  bucket.load(path)

  assert( bucket.value('k1') == 'v1' )
end

test['should return false when the path cannot be written'] = function()
  assert( bucket.dump('/this/dir/does/not/exist/tests-cache-dump.bin') == false )
end

return test
