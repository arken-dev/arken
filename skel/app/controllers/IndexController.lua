local IndexController = Class.new("IndexController", "Controller")

function IndexController:indexAction()
  return self:render{ output = 'html' }
end

return IndexController
