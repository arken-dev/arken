local test = {}
local md5  = require('charon.digest.md5')

test['should return md5 calc'] = function()
  assert( md5.hash('test') == '098f6bcd4621d373cade4e832627b4f6' )
end

return test
