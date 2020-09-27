local url = require('arken.net.url')
local test = {}

test.should_encode_char_with_upcase = function()
  assert(url.encodeUpper("a") == "%61", url.encodeUpper("a"))
end

return test
