local Controller = require 'charon.Controller'
local test = {}

test.should_return_yield_private_var = function()
  local c = Controller.new()
  c._yield = 'Hello ...'
  assert( c:yield() == 'Hello ...' )
end

return test
