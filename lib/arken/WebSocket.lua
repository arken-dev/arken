-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Class = require 'arken.oop.Class'
local cache = require 'arken.cache'
local url   = require 'arken.net.url'
local Room  = require 'arken.net.Room'

local WebSocket = Class.new("WebSocket")

-------------------------------------------------------------------------------
-- CONNECTION
-------------------------------------------------------------------------------

function WebSocket:connection()
  return self._connection
end

-------------------------------------------------------------------------------
-- PARAMS
-- query string do handshake (ex: ws://host/ws/chat/room?id=abc123 -> { id
-- = "abc123" }) - disponível em open/message/close/error, não só no open,
-- porque connection:queryString() vem do Connection do libev-ws (fixo pra
-- vida da conexão), igual path().
-------------------------------------------------------------------------------

function WebSocket:params()
  if self._params == nil then
    self._params = url.parseQuery(self:connection():queryString())
  end
  return self._params
end

-------------------------------------------------------------------------------
-- ROOM
-------------------------------------------------------------------------------

function WebSocket:room(name)
  return Room.new{ name = name, connection = self:connection() }
end

-------------------------------------------------------------------------------
-- SESSION
-------------------------------------------------------------------------------

function WebSocket:session()
  if self._session == nil then
    self._session = cache.value(self:connection():sessionId()) or {}
  end
  return self._session
end

-------------------------------------------------------------------------------
-- EXECUTE
-------------------------------------------------------------------------------

function WebSocket:pexecute(method, ...)
  local status, err = pcall(self[method], self, ...)
  if not status then
    self:rescue(err)
  end
  self:persist()
end

function WebSocket:persist()
  if self._session ~= nil then
    cache.insert(self:connection():sessionId(), self._session)
  end
end

-------------------------------------------------------------------------------
-- RESCUE
-- chamado quando open/message/close lançam exceção (pcall acima), e
-- também direto pelo dispatcher quando o C++ detecta algo que não é uma
-- mensagem normal (violação de protocolo, UTF-8 inválido, mensagem
-- grande demais, timeout de ping) - o motivo vem em err nesse caso.
-------------------------------------------------------------------------------

function WebSocket:rescue(err)
end

return WebSocket
