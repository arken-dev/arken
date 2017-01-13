local OrderController = Class.new("OrderController", "Controller")

function OrderController:saveAction()
  return 200, {'Content-Type: text/html; charset=utf-8'}, 'save !'
end

function OrderController:testAction()

end

return OrderController
