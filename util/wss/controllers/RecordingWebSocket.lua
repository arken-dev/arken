-- fixture pra tests/lib/arken/net/websocketDispatcher/error.lua - grava
-- em RecordingWebSocket.log qual hook foi chamado (error vs rescue),
-- pra provar que os dois caminhos continuam separados
local WebSocket = require 'arken.WebSocket'
local RecordingWebSocket = Class.new("WssTestRecordingWebSocket", "WebSocket")

RecordingWebSocket.log = {}

function RecordingWebSocket:open()
end

function RecordingWebSocket:message(payload, binary)
  if payload == "boom" then
    error("erro de aplicacao de teste")
  end
end

function RecordingWebSocket:close()
end

function RecordingWebSocket:error(reason)
  table.insert(RecordingWebSocket.log, { kind = "error", reason = reason })
end

function RecordingWebSocket:rescue(err)
  table.insert(RecordingWebSocket.log, { kind = "rescue", reason = err })
end

return RecordingWebSocket
