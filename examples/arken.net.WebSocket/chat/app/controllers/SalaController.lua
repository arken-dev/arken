-- Controller HTTP normal (não WebSocket) que só serve a página HTML da
-- sala de chat (ver app/views/sala/index.html). Roteado do jeito comum
-- do arken.dispatcher: GET /app/sala -> SalaController:indexAction()
-- (ver lib/dispatcher.lua e script/server, que liga server:setDispatcher).
--
-- A conexão em si (ws://.../ws/chat/room) é aberta pelo JavaScript da
-- página, não por esse controller - ele só entrega o HTML.

local Controller = require 'arken.Controller'
local SalaController = Class.new("SalaController", "arken.Controller")

function SalaController:indexAction()
  return self:render{ output = "html" }
end

return SalaController
