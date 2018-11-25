local IndexController = Class.new("IndexController", "Controller")

IndexController.layout = "main"

function IndexController:indexAction()
  -- http://localhost:2345/
  return self:render{ output = 'html' }
end

return IndexController
