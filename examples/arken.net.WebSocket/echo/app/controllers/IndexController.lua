-- Controller HTTP normal (não WebSocket) que só serve a página HTML de
-- teste do echo (ver app/views/index/index.html). Roteado do jeito comum
-- do arken.dispatcher: GET / -> IndexController:indexAction() (ver
-- lib/dispatcher.lua e script/server, que liga server:setDispatcher).
--
-- A conexão em si (ws://.../ws/echo) é aberta pelo JavaScript da página,
-- não por esse controller - ele só entrega o HTML.

local Controller = require 'arken.Controller'
local IndexController = Class.new("IndexController", "arken.Controller")

function IndexController:indexAction()
  return self:render{ output = "html" }
end

return IndexController
