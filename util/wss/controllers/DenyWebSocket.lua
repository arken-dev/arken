-- fixture pra tests/lib/arken/net/websocketDispatcher/handshake.lua -
-- sobrescreve handshake() pra recusar sempre, com headers/body customizados
local WebSocket = require 'arken.WebSocket'
local DenyWebSocket = Class.new("WssTestDenyWebSocket", "WebSocket")

function DenyWebSocket:handshake(params)
  return 403, {"X-Test-Reason: negado"}, "acesso negado"
end

function DenyWebSocket:open()
end

function DenyWebSocket:message(payload, binary)
end

function DenyWebSocket:close()
end

return DenyWebSocket
