local test = {}
local sha1 = require('charon.digest.sha1')

test['should return sha1 calc'] = function()
  assert( sha1.hash('test') == 'a94a8fe5ccb19ba61c4c0873d391e987982fbbd3' )
end

return test
