-- arken.net.WebSocketClient example: chat com dois clientes (parte 2/2).
--
-- Roda chat_bob.lua PRIMEIRO, num terminal, e ESSE script depois num
-- outro - os dois conectam na mesma sala do exemplo
-- arken.net.WebSocket/chat e trocam mensagens de verdade entre si. Alice
-- (essa aqui) inicia a conversa assim que conecta; Bob só responde. Cada
-- um responde ao que recebe do outro, num vaivém roteirizado (sem
-- interatividade - ler do teclado bloquearia loop(), ver README ao
-- lado), até acabar as falas de cada um, quando os dois se desconectam
-- sozinhos.
--
-- ATENÇÃO - pré-requisito: precisa do servidor do exemplo
-- arken.net.WebSocket/chat rodando em ws://127.0.0.1:8090. Esse servidor
-- só funciona num build configurado com o backend `libev-ws`
-- (`cmake -DARKEN_NET_HTTPSERVER=libev-ws ..`) - o backend padrão
-- (`libev`) não entende upgrade de WebSocket. Ver README ao lado desse
-- arquivo e o README do exemplo arken.net.WebSocket/chat.

local WebSocketClient = require "arken.net.WebSocketClient"

local client   = WebSocketClient.new("ws://127.0.0.1:8090/ws/chat/room?id=arken-client-demo")
local messages = { "oi Bob! tudo bem?", "que bom que já tava aqui", "beleza, até mais!" }
local sent     = 0

-- manda a próxima fala roteirizada e, se essa era a última, já fecha a
-- conexão - não faz sentido esperar mais nada depois de dizer a última
-- linha, e a outra ponta (chat_bob.lua) faz o mesmo do lado dela
local function sendNext(self)
  sent = sent + 1
  self:send(messages[sent])
  if sent >= #messages then
    self:close("fim do exemplo")
  end
end

client.onOpen = function(self)
  print("[Alice] entrei na sala")
  -- Alice inicia a conversa - Bob (já conectado e esperando) responde
  sendNext(self)
end

client.onMessage = function(self, payload, binary)
  -- entrada/saída de gente na sala também chega aqui, como mensagem de
  -- texto normal (o protocolo desse chat é texto puro, sem JSON - ver
  -- ws/controllers/Chat/RoomWebSocket.lua no exemplo do servidor) - só
  -- mostra, não é uma fala de verdade pra responder
  if payload:contains("entrou na sala") or payload:contains("saiu da sala") then
    print("[Alice] " .. payload)
    return
  end

  print("[Alice] recebi: " .. payload)

  if sent < #messages then
    sendNext(self)
  end
end

client.onClose = function(self, code, reason)
  print(string.format("[Alice] fechado (code=%d, reason=%q)", code, reason))
end

client.onError = function(self, reason)
  print("[Alice] erro: " .. tostring(reason))
end

client:connect()

while client:loop() do
  -- fica em loop contínuo aqui - toda a lógica do exemplo roda dentro
  -- dos callbacks acima, não precisa de nada nesse corpo
end
