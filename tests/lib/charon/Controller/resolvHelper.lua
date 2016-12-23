local Controller = require('charon.Controller')
local tests = {}

tests.should_return_helper_path = function()
  local c = Controller.new{ controller_name = 'controller' }
  c:resolvHelper()
end

return tests
