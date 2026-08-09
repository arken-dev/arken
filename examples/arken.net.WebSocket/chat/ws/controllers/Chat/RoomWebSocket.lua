-- arken.net.WebSocket example: sala de chat completa.
--
-- Path: ws://127.0.0.1:8090/ws/chat/room
-- (o path com barra vira namespace: "chat/room" -> Chat.RoomWebSocket,
-- ver lib/arken/net/websocketDispatcher.lua)
--
-- Mostra as peças que um chat de verdade precisa:
--   - lista de quem está na sala (arken.cache, compartilhado entre threads
--     e mensagens - não dá pra guardar isso numa variável Lua comum)
--   - broadcast: mandar mensagem pra conexões diferentes da que está
--     processando o evento atual (WebSocketConnection.send(sessionId, ...))
--   - sessão por conexão (self:session(), persiste entre mensagens)
--   - rescue(): o app decide o que fazer com timeout de ping/erro de
--     protocolo, não o framework - aqui, 3 timeouts seguidos = kick

local WebSocket           = require 'arken.WebSocket'
local WebSocketConnection = require 'arken.net.WebSocketConnection'
local cache                = require 'arken.cache'

local RoomWebSocket = Class.new("RoomWebSocket", "WebSocket")

-- chave fixa porque esse exemplo só tem uma sala. Uma aplicação com
-- várias salas usaria uma chave por sala (ex: "chat_room_" .. roomId).
local ROOM_KEY = "example_chat_room_members"

local function members()
  return cache.value(ROOM_KEY) or {}
end

local function saveMembers(list)
  cache.insert(ROOM_KEY, list)
end

-------------------------------------------------------------------------------
-- BROADCAST
-------------------------------------------------------------------------------

function RoomWebSocket:broadcast(text)
  for _, sessionId in ipairs(members()) do
    WebSocketConnection.send(sessionId, text)
  end
end

-------------------------------------------------------------------------------
-- OPEN / MESSAGE / CLOSE
-------------------------------------------------------------------------------

function RoomWebSocket:open()
  local list = members()
  table.insert(list, self:connection():sessionId())
  saveMembers(list)

  self:broadcast(self:connection():sessionId() .. " entrou na sala")
end

function RoomWebSocket:message(payload, binary)
  self:broadcast(self:connection():sessionId() .. ": " .. payload)
end

function RoomWebSocket:close()
  local list = members()
  for i, sessionId in ipairs(list) do
    if sessionId == self:connection():sessionId() then
      table.remove(list, i)
      break
    end
  end
  saveMembers(list)

  self:broadcast(self:connection():sessionId() .. " saiu da sala")
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
