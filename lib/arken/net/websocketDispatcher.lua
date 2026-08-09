-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local dispatcher = {}

-------------------------------------------------------------------------------
-- RESOLVE CONTROLLER
-- /wss/usuario/painel -> wss.controllers.Usuario.PainelWebSocket
-------------------------------------------------------------------------------

dispatcher.resolveController = function(path)
  local trimmed = path:mid(2)
  local slash   = trimmed:indexOf('/')
  local root    = trimmed:mid(1, slash - 1)
  local rest    = trimmed:mid(slash + 1)
  local modulePath = root .. ".controllers." .. rest:camelCase() .. "WebSocket"
  return require(modulePath)
end

-------------------------------------------------------------------------------
-- CALL
-------------------------------------------------------------------------------

local call = function(connection, method, ...)
  local class  = dispatcher.resolveController(connection:path())
  local object = class.new{ _connection = connection }

  object:pexecute(method, ...)
end

-------------------------------------------------------------------------------
-- OPEN / MESSAGE / CLOSE
-------------------------------------------------------------------------------

dispatcher.open = function(connection)
  call(connection, "open")
end

dispatcher.message = function(connection, payload, binary)
  call(connection, "message", payload, binary)
end

dispatcher.close = function(connection)
  call(connection, "close")
end

return dispatcher
