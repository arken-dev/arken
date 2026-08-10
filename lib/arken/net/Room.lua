-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

-- arken.net.Room: membership de sala pra WebSocket, sobre arken.cache.
--
-- Cada sala vira um bucket próprio (cache.bucket("room:" .. name)) - todo
-- Room.new{ name = "chat/123" } criado em qualquer conexão/thread aponta
-- pro mesmo bucket compartilhado, porque cache.bucket() indexa por nome
-- num registro do processo (ver arken::cache::s_registry). add/leave usam
-- uma chave por sessionId (insert/remove), não uma lista JSON única lida e
-- regravada inteira - isso evita a race de dois open()/close() concorrentes
-- pisando um no outro ao salvar a lista completa.

local Class               = require 'arken.oop.Class'
local cache                = require 'arken.cache'
local WebSocketConnection  = require 'arken.net.WebSocketConnection'

local Room = Class.new("arken.net.Room")

function Room:initialize()
  self._bucket = cache.bucket("room:" .. self.name)
end

-------------------------------------------------------------------------------
-- MEMBERSHIP
-------------------------------------------------------------------------------

function Room:add(sessionId)
  sessionId = sessionId or self.connection:sessionId()
  -- expires = 0 (nunca expira por tempo, ver src/arken/cache.cpp) -
  -- sem isso, arken.cache usa 60s de default: uma sala parada por 60s+
  -- sem ninguém entrar/sair perdia os membros silenciosamente, e
  -- broadcast() passava a não mandar mensagem pra ninguém. Membership
  -- some por leave() (chamado em close()), nunca por TTL.
  self._bucket.insert(sessionId, true, 0)
end

function Room:leave(sessionId)
  sessionId = sessionId or self.connection:sessionId()
  self._bucket.remove(sessionId)
end

function Room:members()
  return self._bucket.keys()
end

-------------------------------------------------------------------------------
-- BROADCAST
-- por padrão não ecoa pra quem está mandando (self.connection, quando o
-- Room veio de self:room() numa WebSocket) - pra incluir o remetente,
-- passe includeSelf = true.
-------------------------------------------------------------------------------

function Room:broadcast(payload, binary, includeSelf)
  local exclude = nil
  if not includeSelf and self.connection then
    exclude = self.connection:sessionId()
  end

  for _, sessionId in ipairs(self:members()) do
    if sessionId ~= exclude then
      WebSocketConnection.send(sessionId, payload, binary)
    end
  end
end

return Room
