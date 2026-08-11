-- arken.net.WebSocket example: sala de chat completa.
--
-- Path: ws://127.0.0.1:8090/ws/chat/room?id=<nome-da-sala>
-- (o path com barra vira namespace: "chat/room" -> Chat.RoomWebSocket,
-- ver lib/arken/net/websocketDispatcher.lua - o "?id=..." não entra nessa
-- conta, é query string, resolvida à parte por self:params())
--
-- Mostra as peças que um chat de verdade precisa:
--   - qual sala: self:params().id, vindo da query string do handshake
--     (self:connection():queryString(), parseada por WebSocket:params())
--   - lista de quem está na sala (lib/Room.lua, classe de exemplo desse
--     chat sobre arken.cache - compartilhado entre threads e mensagens,
--     uma chave por membro em vez de uma lista JSON única lida/regravada
--     inteira)
--   - broadcast: mandar mensagem pra conexões diferentes da que está
--     processando o evento atual (self:room():broadcast() não ecoa pra
--     quem mandou por padrão)
--   - sessão por conexão (self:session(), persiste entre mensagens)
--   - error(): o app decide o que fazer com timeout de ping/erro de
--     protocolo, não o framework - aqui, 3 timeouts seguidos = kick
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
--                               open()    <- primeira vez que fala com o client
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

local Room = require 'lib.Room'

local RoomWebSocket = Class.new("RoomWebSocket", "WebSocket")

-- nome default pra quem conectar sem "?id=..." (ex: abrindo a URL crua).
-- Uma aplicação de verdade decidiria se isso devia ser permitido ou não.
local DEFAULT_ROOM_NAME = "example_chat_room"

function RoomWebSocket:room()
  return Room.new{ name = self:params().id or DEFAULT_ROOM_NAME, connection = self:connection() }
end

-------------------------------------------------------------------------------
-- HANDSHAKE (HTTP)
-- roda antes do 101 - ainda é HTTP normal aqui, a conexão nem virou
-- WebSocket ainda. É o lugar certo pra validar/recusar (ex: autenticação,
-- permissão pra entrar na sala) antes de aceitar o upgrade. Esse exemplo
-- aceita sempre; comentado abaixo, um exemplo de como recusar.
-------------------------------------------------------------------------------

function RoomWebSocket:handshake(params)
  print(self:params().id)
  -- if autenticacaoUsuario() == false then
  --   return 403, {}, "sem permissão pra entrar nessa sala"
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
function RoomWebSocket:open()
  local room = self:room()
  room:add()
  -- includeSelf = true: quem entrou também vê o próprio aviso de entrada
  room:broadcast(self:connection():sessionId() .. " entrou na sala", false, true)
end

-- dispara a cada frame de texto/binário completo recebido do cliente
function RoomWebSocket:message(payload, binary)
  local time = Time.now():toString()
  local session_id = self:connection():sessionId()
  local message = string.format("%s#%s: %s", session_id, time, payload)
  -- broadcast exclui o remetente por padrão - quem manda já sabe o que
  -- escreveu, o cliente faz o eco local se quiser mostrar na hora
  self:room():broadcast(message)
end

-- dispara quando a conexão termina, de qualquer jeito (cliente fechou,
-- rede caiu, servidor derrubou) - último momento pra liberar recursos
function RoomWebSocket:close()
  local room = self:room()
  room:leave()
  room:broadcast(self:connection():sessionId() .. " saiu da sala", false, true)
end

-- erro de protocolo WebSocket detectado pelo C++ (timeout de ping,
-- UTF-8 inválido, mensagem grande demais, violação de protocolo) -
-- reason distingue os casos. Não é exceção de aplicação - isso é
-- rescue(), logo abaixo.
function RoomWebSocket:error(reason)
  if reason == "timeout" then
    local session = self:session()
    session.timeouts = (session.timeouts or 0) + 1

    if session.timeouts >= 3 then
      -- 3 pings seguidos sem resposta: decide que a conexão está morta e
      -- avisa o cliente antes de deixar o encerramento acontecer pelo
      -- caminho normal - esse "3" é decisão da aplicação, não do
      -- framework, por isso mora aqui e não em C++
      self:connection():close("sem resposta ao ping")
    end
  else
    -- "protocol" / "invalid_utf8" / "too_large"
    print("[RoomWebSocket] error: " .. tostring(reason) ..
      " (session=" .. self:connection():sessionId() .. ")")
  end
end

-------------------------------------------------------------------------------
-- RESCUE
-- erro de aplicação: exceção Lua real lançada dentro de open/message/close
-- (capturada pelo pcall de WebSocket:pexecute) - err é a mensagem do erro,
-- não uma das strings fixas de error() acima.
-------------------------------------------------------------------------------

function RoomWebSocket:rescue(err)
  print("[RoomWebSocket] rescue: " .. tostring(err) ..
    " (session=" .. self:connection():sessionId() .. ")")
end

return RoomWebSocket
