local helper = require 'charon.Helper'
local test   = {}

test.return_self = function()
  assert( helper:dateTimeFormat("mydate") == "mydate" )
end

return test
