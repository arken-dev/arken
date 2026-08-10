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
--   - lista de quem está na sala (arken.net.Room, sobre arken.cache -
--     compartilhado entre threads e mensagens, uma chave por membro em
--     vez de uma lista JSON única lida/regravada inteira)
--   - broadcast: mandar mensagem pra conexões diferentes da que está
--     processando o evento atual (self:room():broadcast() não ecoa pra
--     quem mandou por padrão)
--   - sessão por conexão (self:session(), persiste entre mensagens)
--   - rescue(): o app decide o que fazer com timeout de ping/erro de
--     protocolo, não o framework - aqui, 3 timeouts seguidos = kick

local RoomWebSocket = Class.new("RoomWebSocket", "WebSocket")

-- nome default pra quem conectar sem "?id=..." (ex: abrindo a URL crua).
-- Uma aplicação de verdade decidiria se isso devia ser permitido ou não.
local DEFAULT_ROOM_NAME = "example_chat_room"

function RoomWebSocket:room()
  return self:super("room", self:params().id or DEFAULT_ROOM_NAME)
end

-------------------------------------------------------------------------------
-- OPEN / MESSAGE / CLOSE
-------------------------------------------------------------------------------

function RoomWebSocket:open()
  local room = self:room()
  room:add()
  -- includeSelf = true: quem entrou também vê o próprio aviso de entrada
  room:broadcast(self:connection():sessionId() .. " entrou na sala", false, true)
end

function RoomWebSocket:message(payload, binary)
  local time = Time.now():toString()
  local session_id = self:connection():sessionId()
  local message = string.format("%s#%s: %s", session_id, time, payload)
  -- broadcast exclui o remetente por padrão - quem manda já sabe o que
  -- escreveu, o cliente faz o eco local se quiser mostrar na hora
  self:room():broadcast(message)
end

function RoomWebSocket:close()
  local room = self:room()
  room:leave()
  room:broadcast(self:connection():sessionId() .. " saiu da sala", false, true)
end

-------------------------------------------------------------------------------
-- RESCUE
-- chamado pra exceção Lua de verdade E pra timeout de ping/violação de
-- protocolo (ver lib/arken/net/websocketDispatcher.lua) - reason distingue
-- os casos.
-------------------------------------------------------------------------------

function RoomWebSocket:rescue(reason)
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
    -- "protocol" / "invalid_utf8" / "too_large", ou uma exceção Lua de
    -- verdade (nesse caso reason é o erro do pcall, não uma string fixa)
    print("[RoomWebSocket] rescue: " .. tostring(reason) ..
      " (session=" .. self:connection():sessionId() .. ")")
  end
end

return RoomWebSocket
