local dispatcher = require 'arken.net.websocketDispatcher'
local test = {}

test.should_build_module_path_from_single_segment_path = function()
  assert(dispatcher.modulePath("/wss/echo") == "wss.controllers.EchoWebSocket")
end

test.should_build_module_path_from_nested_path = function()
  assert(dispatcher.modulePath("/wss/chat/room") == "wss.controllers.Chat.RoomWebSocket")
end

test.should_be_used_consistently_by_resolveController = function()
  -- resolveController precisa continuar resolvendo pro mesmo módulo que
  -- modulePath calcula, já que dispatch (open/message/close) depende disso
  package.path = package.path .. ';util/?.lua'
  local class = dispatcher.resolveController("/wss/echo")
  assert(class.className == "WssTestEchoWebSocket", class.className)
end

return test
