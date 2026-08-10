-- arken.net.WebSocket example: o mais simples possível.
--
-- Path: ws://127.0.0.1:8090/ws/echo
--
-- Todo controller WebSocket estende arken.WebSocket e implementa
-- open/message/close (e opcionalmente rescue, ver Chat/RoomWebSocket.lua
-- pra um exemplo com isso). self:connection() dá acesso à conexão que
-- disparou o evento atual - send() manda uma mensagem de volta pra ela.

local WebSocket = require 'arken.WebSocket'
local EchoWebSocket = Class.new("EchoWebSocket", "WebSocket")

function EchoWebSocket:open()
  self:connection():send("conectado! session_id=" .. self:connection():sessionId())
end

function EchoWebSocket:message(payload, binary)
  self:connection():send(payload, binary)
end

function EchoWebSocket:close()
  -- nada a fazer aqui nesse exemplo - mas é o lugar certo pra liberar
  -- qualquer recurso associado à conexão (ex: tirar a pessoa de uma sala)
end

return EchoWebSocket
