local cache = require('arken.cache')
local mvm   = require('arken.mvm')

local test = {}

local bucketName = 'tests-cache-load'
local bucket      = cache.bucket(bucketName)
local path        = mvm.path() .. '/tmp/tmp/tests-cache-load.bin'

test.after = function()
  for _, k in ipairs(bucket.keys('*')) do
    bucket.remove(k)
  end
  os.remove(path)
end

test['should return false for a file that does not exist'] = function()
  assert( bucket.load('/tmp/tests-cache-load-does-not-exist.bin') == false )
end

test['should preserve remaining ttl and skip keys already expired at load time'] = function()
  bucket.insert('long', 'still-here', 3600)
  bucket.insert('forever', 'never-expires', 0)
  bucket.insert('short', 'about-to-die', 1)

  assert( bucket.dump(path) )

  -- 'short' expira de verdade antes do load(), pra confirmar que uma chave
  -- já expirada não é restaurada
  os.sleep(2)

  for _, k in ipairs(bucket.keys('*')) do
    bucket.remove(k)
  end
  assert( #bucket.keys('*') == 0 )

  assert( bucket.load(path) )

  assert( bucket.value('long') == 'still-here' )
  assert( bucket.value('forever') == 'never-expires' )
  assert( bucket.value('short') == nil )
end

return test
