local test = {}
local dispatcher = require('charon.dispatcher')
package.path = package.path .. ';util/?.lua'

test.should_return_require_controller_with_camelcase = function()
  local class = dispatcher.requireController('util')
end

return test
