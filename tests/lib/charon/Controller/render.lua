local Controller = require('charon.Controller')
local test = {}

test.should_execute_output_method_prefix_render = function()
  Controller.render_mycustomrender = function()
    return "works !!!"
  end
  local c = Controller.new()
  local result = c:render{ output = 'mycustomrender' }
  Controller.render_mycustomrender = nil
  assert( result == "works !!!" )
end

return test
