-- Controller HTTP normal (não WebSocket) que só serve a página HTML de
-- teste do chat (ver app/views/index/index.html). Roteado do jeito
-- comum do arken.dispatcher: GET / -> IndexController:indexAction() (ver
-- lib/dispatcher.lua e script/server, que liga server:setDispatcher).
--
-- A conexão em si (ws://.../ws/chat/room) é aberta pelo JavaScript da
-- página, não por esse controller - ele só entrega o HTML.

local Controller = require 'arken.Controller'
local IndexController = Class.new("IndexController", "arken.Controller")

function IndexController:indexAction()
  return self:render{ output = "html" }
end

function IndexController:reloadAction()
  local mvm = require('arken.mvm')
  mvm.reload()
  return 200, {}, "reload..."
end

return IndexController
