local IndexController = Class.new("IndexController", "arken.Controller")

function IndexController:indexAction()
  return 200, {}, "index..."
end

function IndexController:reloadAction()
  local mvm = require('arken.mvm')
  mvm.reload()
  return 200, {}, "reload..."
end

return IndexController
