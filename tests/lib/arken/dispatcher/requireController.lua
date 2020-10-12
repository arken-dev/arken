local dispatcher = require('arken.dispatcher')
local test = {}
package.path = package.path .. ';util/?.lua'

test.should_return_require_controller_with_camel_case = function()
  local controller = dispatcher.requireController('util')
  assert(controller.className == 'UtilController', controller.className )
end

return test
