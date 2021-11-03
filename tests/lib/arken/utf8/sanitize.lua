local utf8 = require('arken.utf8')
local test = {}

test['should clean negative chars in the string'] = function()
  local str = "hell�!�"
  local res = utf8.sanitize(str)
  assert( res == "hell!" )
end

return test
