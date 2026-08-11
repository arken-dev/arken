-- fixture pra tests/lib/arken/net/websocketDispatcher/handshake.lua - não
-- sobrescreve handshake(), então herda o default de arken.WebSocket (200)
local WebSocket = require 'arken.WebSocket'
local EchoWebSocket = Class.new("WssTestEchoWebSocket", "WebSocket")

function EchoWebSocket:open()
end

function EchoWebSocket:message(payload, binary)
end

function EchoWebSocket:close()
end

return EchoWebSocket
