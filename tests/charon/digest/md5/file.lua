local test = {}
local md5  = require('arken.digest.md5')

test['should return md5 calc'] = function()
  local result = md5.file('util/tests/digest/file.txt')
  assert( result == 'd6b429978a3e0e6c4133093c7b1cccae', result )
end

return test
