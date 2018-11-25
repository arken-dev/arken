local HelloWorldController  = Class.new("HelloWorldController", "Controller")
HelloWorldController.layout = "main"

function HelloWorldController:indexAction()
  -- http://localhost:2345/helloWorld
  self.message = "Hello World"
  return self:render{ output = 'html' }
end

return HelloWorldController
