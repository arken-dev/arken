local test = {}
local dispatcher = require('charon.dispatcher')
package.path = package.path .. ';util/?.lua'

test.should_return_require_controller_with_camelcase = function()
  local controller = dispatcher.requireController('util')
  assert(controller.className == 'UtilController', controller.className )
end

return test
