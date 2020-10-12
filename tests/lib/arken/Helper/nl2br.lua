local helper = require 'arken.Helper'
local test   = {}

test.return_self = function()
  assert( helper:nl2br("first\nsecond") == "first<br>second" )
end

return test
