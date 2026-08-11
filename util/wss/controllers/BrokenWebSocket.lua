-- fixture pra tests/lib/arken/net/websocketDispatcher/handshake.lua -
-- explode na instanciação (initialize), simulando um controller com bug
-- de carregamento - dispatcher.handshake deve virar 500, não propagar o erro
local WebSocket = require 'arken.WebSocket'
local BrokenWebSocket = Class.new("WssTestBrokenWebSocket", "WebSocket")

function BrokenWebSocket:initialize()
  error("boom - falha proposital de instanciação")
end

return BrokenWebSocket
