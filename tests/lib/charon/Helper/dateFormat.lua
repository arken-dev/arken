local helper = require 'arken.Helper'
local test   = {}

test.return_self = function()
  assert( helper:dateFormat("mydate") == "mydate" )
end

return test
