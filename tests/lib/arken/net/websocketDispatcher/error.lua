-- cobre a separação entre erro de protocolo WebSocket (dispatcher.error
-- -> object:onError()) e erro de aplicação/lógica (exceção Lua real dentro
-- de onOpen/onMessage/onClose, capturada por pexecute -> object:rescue()) -
-- antes das duas caírem em rescue(), o que misturava os dois conceitos
local dispatcher = require 'arken.net.websocketDispatcher'
local test = {}

package.path = package.path .. ';util/?.lua'

local RecordingWebSocket = require 'wss.controllers.RecordingWebSocket'

local mockConnection = function(path)
  local connection = {}
  connection.path        = function() return path end
  connection.queryString = function() return "" end
  return connection
end

test.before = function()
  RecordingWebSocket.log = {}
end

test.should_call_error_not_rescue_for_protocol_errors = function()
  dispatcher.error(mockConnection("/wss/recording"), "timeout")

  assert(#RecordingWebSocket.log == 1, #RecordingWebSocket.log)
  assert(RecordingWebSocket.log[1].kind == "error", RecordingWebSocket.log[1].kind)
  assert(RecordingWebSocket.log[1].reason == "timeout", RecordingWebSocket.log[1].reason)
end

test.should_call_rescue_not_error_for_application_exceptions = function()
  dispatcher.message(mockConnection("/wss/recording"), "boom", false)

  assert(#RecordingWebSocket.log == 1, #RecordingWebSocket.log)
  assert(RecordingWebSocket.log[1].kind == "rescue", RecordingWebSocket.log[1].kind)
end

test.should_not_call_error_when_message_does_not_throw = function()
  dispatcher.message(mockConnection("/wss/recording"), "hello", false)

  assert(#RecordingWebSocket.log == 0, #RecordingWebSocket.log)
end

return test
