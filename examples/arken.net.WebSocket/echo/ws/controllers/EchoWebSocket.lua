-- arken.net.WebSocket example: echo com prova de round-trip pelo servidor.
--
-- Path: ws://127.0.0.1:8091/ws/echo
--
-- Diferente do echo mais simples possível (que só manda de volta
-- exatamente o payload recebido), esse aqui embrulha a resposta com
-- informação que só existe do lado do servidor (sessionId, timestamp) -
-- assim quem está testando consegue confirmar visualmente que o texto
-- não voltou "por conta própria" no client, ele realmente foi até o
-- servidor e voltou (ver app/views/index/index.html pro lado JS disso).
--
-- Fluxo desse exemplo, do handshake até o fim da conexão:
--
--                       HTTP
--                        │
--                        ▼
--                  handshake(params)     <- ainda é HTTP normal aqui
--                        │
--               ┌────────┴────────┐
--               │                 │
--            recusa             aceita
--               │                 │
--               ▼                 ▼
--          403 (ou outro)        101     <- a partir daqui é WebSocket
--                                 │
--                                 ▼
--                              open()    <- primeira vez que fala com o client
--                                 │
--                       ┌─────────┴─────────┐
--                       │                   │
--                  message()             error()   <- mensagem normal x erro de protocolo
--                       │                   │
--                       └─────────┬─────────┘
--                                 │
--                                 ▼
--                              close()   <- fim da conexão, de qualquer jeito
--
-- rescue() não aparece nesse fluxo - é um desvio à parte: se open(),
-- message() ou close() lançarem uma exceção Lua de verdade (bug da
-- aplicação, não erro de protocolo), cai em rescue() em vez de seguir
-- o fluxo normal.

local json = require 'arken.json'
local WebSocket = require 'arken.WebSocket'
local EchoWebSocket = Class.new("EchoWebSocket", "WebSocket")

-------------------------------------------------------------------------------
-- HANDSHAKE (HTTP)
-- roda antes do 101 - ainda é HTTP normal aqui, a conexão nem virou
-- WebSocket ainda. É o lugar certo pra validar/recusar (ex: autenticação,
-- limite de conexões) antes de aceitar o upgrade. Esse exemplo aceita
-- sempre; comentado abaixo, um exemplo de como recusar.
-------------------------------------------------------------------------------

function EchoWebSocket:handshake(params)
  -- if autenticacaoUsuario() == false then
  --   return 403, {}, "sem permissão"
  -- end
  return 200, {}, ""
end

-------------------------------------------------------------------------------
-- OPEN / MESSAGE / CLOSE / ERROR (WEBSOCKET)
-- a partir daqui a conexão já é WebSocket de verdade - o 101 já foi
-- respondido, handshake() já aprovou
-------------------------------------------------------------------------------

-- dispara uma vez, assim que a conexão vira WebSocket - primeiro momento
-- em que dá pra falar com esse cliente
function EchoWebSocket:open()
  self:connection():send(json.encode{
    event     = "open",
    sessionId = self:connection():sessionId(),
  })
end

-- dispara a cada frame de texto/binário completo recebido do cliente
function EchoWebSocket:message(payload, binary)
  -- só embrulha em JSON se for texto - frame binário não tem por que
  -- virar string JSON, ecoa cru como sempre
  if binary then
    self:connection():send(payload, true)
    return
  end

  self:connection():send(json.encode{
    event     = "echo",
    sessionId = self:connection():sessionId(),
    timestamp = os.date("%H:%M:%S"),
    payload   = payload,
  })
end

-- dispara quando a conexão termina, de qualquer jeito (cliente fechou,
-- rede caiu, servidor derrubou) - último momento pra liberar recursos.
-- nada a fazer aqui nesse exemplo (não tem sala/sessão pra limpar)
function EchoWebSocket:close()
end

-- erro de protocolo WebSocket detectado pelo C++ (timeout de ping,
-- UTF-8 inválido, mensagem grande demais, violação de protocolo) -
-- reason distingue os casos. Não é exceção de aplicação - isso é
-- rescue(), logo abaixo. Esse exemplo só loga; uma aplicação real
-- decidiria o que fazer (ver Chat/RoomWebSocket.lua pra um exemplo que
-- derruba a conexão depois de N timeouts de ping seguidos)
function EchoWebSocket:error(reason)
  print("[EchoWebSocket] error: " .. tostring(reason) ..
    " (session=" .. self:connection():sessionId() .. ")")
end

-------------------------------------------------------------------------------
-- RESCUE
-- erro de aplicação: exceção Lua real lançada dentro de open/message/close
-- (capturada pelo pcall de WebSocket:pexecute) - err é a mensagem do erro,
-- não uma das strings fixas de error() acima.
-------------------------------------------------------------------------------

function EchoWebSocket:rescue(err)
  print("[EchoWebSocket] rescue: " .. tostring(err) ..
    " (session=" .. self:connection():sessionId() .. ")")
end

return EchoWebSocket
