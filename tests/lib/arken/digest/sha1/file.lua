local test = {}
local sha1 = require('arken.digest.sha1')

test['should return sha1 calc'] = function()
  local result = sha1.file('util/tests/digest/file.txt')
  assert( result == 'cda763ecf54539194cba4d7f17a4cbf253cbacfa', #result )
end

return test
