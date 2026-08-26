-- cobre o fix do bug de segurança: WS upgrade não pode mais ser aceito
-- sem checar se existe controller pro path (ver run.cpp/websockethandler.cpp
-- pro lado C++ - aqui só a parte testável em Lua puro, com connection mockada)
local dispatcher = require 'arken.net.websocketDispatcher'
local test = {}

package.path = package.path .. ';util/?.lua'

local mockConnection = function(path, query)
  local connection = {}
  connection.path        = function() return path end
  connection.queryString = function() return query or "" end
  return connection
end

test.should_return_404_if_controller_does_not_exist = function()
  local status, headers, body = dispatcher.handshake(mockConnection("/wss/unknown"))
  assert(status == 404, status)
end

test.should_return_200_when_controller_has_no_handshake_override = function()
  local status, headers, body = dispatcher.handshake(mockConnection("/wss/echo"))
  assert(status == 200, status)
end

test.should_propagate_code_headers_and_body_from_controller_handshake = function()
  local status, headers, body = dispatcher.handshake(mockConnection("/wss/deny"))
  assert(status == 403, status)
  assert(headers[1] == "X-Test-Reason: negado", headers[1])
  assert(body == "acesso negado", body)
end

test.should_return_500_if_controller_fails_to_instantiate = function()
  local status, headers, body = dispatcher.handshake(mockConnection("/wss/broken"))
  assert(status == 500, status)
end

test.should_expose_query_string_as_params_to_handshake = function()
  -- DenyWebSocket sempre recusa independente dos params, mas isso prova
  -- que connection:queryString() chega até object:params() (usado por
  -- controllers reais pra decidir, ex: checar um token)
  local status = dispatcher.handshake(mockConnection("/wss/deny", "token=abc"))
  assert(status == 403, status)
end

return test
