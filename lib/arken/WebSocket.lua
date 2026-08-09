-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Class = require 'arken.oop.Class'
local cache = require 'arken.cache'

local WebSocket = Class.new("WebSocket")

-------------------------------------------------------------------------------
-- CONNECTION
-------------------------------------------------------------------------------

function WebSocket:connection()
  return self._connection
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
    self:error(err)
  end
  if self._session ~= nil then
    cache.insert(self:connection():sessionId(), self._session)
  end
end

-------------------------------------------------------------------------------
-- ERROR
-------------------------------------------------------------------------------

function WebSocket:error(err)
end

return WebSocket
