local md5  = require('arken.digest.md5')
local test = {}

test['should return md5 calc'] = function()
  assert( md5.hash('test') == '098f6bcd4621d373cade4e832627b4f6' )
end

return test
