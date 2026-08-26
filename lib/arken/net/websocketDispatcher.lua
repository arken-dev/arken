-- Copyright 2016 The Arken Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local dispatcher = {}

-------------------------------------------------------------------------------
-- RESOLVE CONTROLLER
-- /wss/usuario/painel -> wss.controllers.Usuario.PainelWebSocket
-------------------------------------------------------------------------------

dispatcher.modulePath = function(path)
  local trimmed = path:mid(2)
  local slash   = trimmed:indexOf('/')
  local root    = trimmed:mid(1, slash - 1)
  local rest    = trimmed:mid(slash + 1)
  return root .. ".controllers." .. rest:camelCase() .. "WebSocket"
end

dispatcher.resolveController = function(path)
  return require(dispatcher.modulePath(path))
end

-------------------------------------------------------------------------------
-- HANDSHAKE
-- roda antes do handshake (101) ser respondido - decide se a conexão pode
-- ser aceita. 200 libera o upgrade (headers/body são descartados nesse
-- caso - a resposta do upgrade é fixa pela RFC); qualquer outro código
-- vira a resposta HTTP enviada no lugar do 101 (404 = controller não
-- existe, 500 = erro ao carregar/instanciar - ambos automáticos; qualquer
-- outro código/headers/body é decisão do object:handshake() do
-- controller, ex: 403 recusando por permissão, ou 302 com Location pra
-- redirecionar)
-------------------------------------------------------------------------------

dispatcher.handshake = function(connection)
  local modulePath = dispatcher.modulePath(connection:path())

  if not package.searchpath(modulePath, package.path) then
    return 404, {}, ""
  end

  local ok, classOrErr = pcall(require, modulePath)
  if not ok then
    return 500, {}, ""
  end

  -- essa instância só existe pra rodar handshake() - open() cria uma
  -- segunda instância própria logo depois (initialize() roda duas vezes;
  -- inofensivo hoje porque nenhum controller sobrescreve initialize(), mas
  -- side effects de conexão pertencem a open(), não a initialize())
  local ok2, objectOrErr = pcall(classOrErr.new, { _connection = connection })
  if not ok2 then
    return 500, {}, ""
  end

  return objectOrErr:handshake(objectOrErr:params())
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
  call(connection, "onOpen")
end

dispatcher.message = function(connection, payload, binary)
  call(connection, "onMessage", payload, binary)
end

dispatcher.close = function(connection)
  call(connection, "onClose")
end

-------------------------------------------------------------------------------
-- ERROR
-- erro de protocolo WebSocket detectado pelo C++ (timeout de ping, UTF-8
-- inválido, mensagem grande demais, violação de protocolo) - não é
-- exceção de aplicação, por isso chama object:onError() e não
-- object:rescue() (reservado pra exceção Lua real, capturada por
-- pexecute). Não passa por pexecute/pcall de novo aqui (evita recursão
-- se o próprio onError() falhar).
-------------------------------------------------------------------------------

dispatcher.error = function(connection, reason)
  local class  = dispatcher.resolveController(connection:path())
  local object = class.new{ _connection = connection }
  object:onError(reason)
  object:persist()
end

return dispatcher
