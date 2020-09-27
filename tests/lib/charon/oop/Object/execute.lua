local Object = require('arken.oop.Object')
local test   = {}

test.should_execute_generic_contract = function()
  local obj = Object.new()
  function obj:cancel()
    return 'cancel'
  end
  assert( obj:execute('cancel') == 'cancel' )
end

return test
