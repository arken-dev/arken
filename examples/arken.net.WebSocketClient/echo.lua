-- arken.net.WebSocketClient example: conecta no servidor do exemplo
-- arken.net.WebSocket/echo, manda algumas mensagens e mostra o
-- round-trip completo (sessionId/timestamp que só existem porque a
-- mensagem realmente foi até o servidor arken e voltou - ver
-- ws/controllers/EchoWebSocket.lua nesse outro exemplo).
--
-- ATENÇÃO - pré-requisito: precisa do servidor do exemplo
-- arken.net.WebSocket/echo rodando em ws://127.0.0.1:8091. Esse servidor
-- só funciona num build configurado com o backend `libev-ws`
-- (`cmake -DARKEN_NET_HTTPSERVER=libev-ws ..`) - o backend padrão
-- (`libev`) não entende upgrade de WebSocket. Ver README ao lado desse
-- arquivo e o README do exemplo arken.net.WebSocket/echo.

local json            = require "arken.json"
local WebSocketClient = require "arken.net.WebSocketClient"

local client   = WebSocketClient.new("ws://127.0.0.1:8091/ws/echo")
local messages = { "oi", "tudo bem?", "última mensagem" }
local sent     = 0

client.onOpen = function(self)
  print("[client] conexão WebSocket estabelecida")
end

client.onMessage = function(self, payload, binary)
  local data = json.decode(payload)

  -- primeira mensagem que chega é o "open" que o servidor manda assim
  -- que a conexão vira WebSocket (ver EchoWebSocket:open) - ainda não é
  -- eco de nada, é só a confirmação da sessão
  if data.event == "open" then
    print("[client] servidor confirmou sessão " .. data.sessionId)
    sent = 1
    self:send(messages[sent])
    return
  end

  print(string.format("[client] eco de %s às %s: %s",
    data.sessionId, data.timestamp, data.payload))

  if sent < #messages then
    sent = sent + 1
    self:send(messages[sent])
  else
    self:close("fim do exemplo")
  end
end

client.onClose = function(self, code, reason)
  print(string.format("[client] fechado (code=%d, reason=%q)", code, reason))
end

client.onError = function(self, reason)
  print("[client] erro: " .. tostring(reason))
end

client:connect()

-- client:loop() bloqueia até ter um evento pra entregar (onOpen/
-- onMessage/onClose/onError, definidos lá em cima) ou a conexão fechar,
-- e retorna true enquanto ainda há conexão. Só depois que o onClose
-- final já foi entregue (disparado por self:close() no onMessage, ou por
-- um erro) é que ele retorna false, terminando esse while sozinho.
while client:loop() do
  -- fica em loop contínuo aqui - toda a lógica do exemplo roda dentro
  -- dos callbacks acima, não precisa de nada nesse corpo
end
