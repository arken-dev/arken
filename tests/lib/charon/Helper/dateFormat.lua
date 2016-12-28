local helper = require 'charon.Helper'
local test   = {}

test.return_self = function()
  assert( helper:dateFormat("mydate") == "mydate" )
end

return test
