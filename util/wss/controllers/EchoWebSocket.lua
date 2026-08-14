-- fixture pra tests/lib/arken/net/websocketDispatcher/handshake.lua - não
-- sobrescreve handshake(), então herda o default de arken.WebSocket (200)
local WebSocket = require 'arken.WebSocket'
local EchoWebSocket = Class.new("WssTestEchoWebSocket", "WebSocket")

function EchoWebSocket:onOpen()
end

function EchoWebSocket:onMessage(payload, binary)
end

function EchoWebSocket:onClose()
end

return EchoWebSocket
