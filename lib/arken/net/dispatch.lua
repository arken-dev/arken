local dispatcher = require('arken.dispatcher')
local env        = require('arken.mvm').env()
local max        = (env == 'development') and 1 or 10
local log        = require('arken.Log').new(env, max)

if env == 'development' then
  dispatcher.output = function(str)
    print(str)
    log:debug(str)
  end
else
  dispatcher.output = function(str)
    log:debug(str)
  end
end

-------------------------------------------------------------------------------
-- DISPATCH
-------------------------------------------------------------------------------

dispatcher.dispatch = function(_env)
  dispatcher.before()
  local time    = os.microtime()
  local reload  = 0
  local code, headers, body
  if env == 'development' then
    local requestPath = _env:requestPath()
    if #requestPath == 5 then
      requestPath = requestPath .. 'index'
    end
    if #requestPath == 4 then
      requestPath = requestPath .. '/index'
    end
    local fileName    = dispatcher.public .. requestPath:mid(#dispatcher.prefix+1, -1)
    if requestPath ~= '/' and os.exists(fileName) then
      return dispatcher.dispatchLocal(fileName)
    else
      reload = package.reload()
      code, headers, body = dispatcher.dispatchController(_env)
    end
  else
    code, headers, body = dispatcher.dispatchController(_env)
  end
  time = (os.microtime() - time)
  if code == nil then
    error "body empty, missing render or return ?"
  end
  dispatcher.log(_env, code, time, reload)
  dispatcher.after()
  return code, headers, body
end

-------------------------------------------------------------------------------
-- PARSE PATH
-------------------------------------------------------------------------------

dispatcher.parsePath  = function(_env)
  local path   = _env:requestPath()
  -- support for rest path example /app/pedido/1234/iteracoes
  -- remove number e return /app/pedido/iteracoes

  local last   = path:lastIndexOf('/')
  local start  = 1
  local prefix = path:mid(2, path:indexOf('/', 2)-2)
  if #prefix == 0 then
    prefix = 'app'
  end
  start = start + #prefix + 2
  local controllerName = path:mid(start, last-start)
  local actionName     = path:mid(#controllerName + start + 1)
  local controllerPath = path:mid(1, path:len() - actionName:len())
  if controllerName == '' then
    controllerName = 'index'
  end

  if last == 1 or actionName == '' then
    actionName = "index"
  end

  return prefix, controllerName, actionName:camelCase(true), controllerPath
end

-------------------------------------------------------------------------------
-- REQUIRE CONTROLLER
-------------------------------------------------------------------------------

dispatcher.requireController = function(prefix, controllerName)
  return require(prefix .. '.controllers.' .. controllerName:camelCase() .. "Controller")
end

-------------------------------------------------------------------------------
-- DISPATCHER CONTROLLER
-------------------------------------------------------------------------------

dispatcher.dispatchController = function(_env)
  local prefix, controllerName, actionName, controllerPath = dispatcher.parsePath(_env)
  local class  = dispatcher.requireController(prefix, controllerName)
  local object = class.new{
    controllerName = controllerName,
    actionName     = actionName,
    controllerPath = controllerPath,
    _env           = _env
  }
  if object[actionName .. "Action"] then
    return object:pexecute(actionName .. "Action")
  else
    if object["intercept"] then
      return object:pexecute("intercept")
    else
      return 500, {}, "action: \"" .. actionName .. "Action\" not found"
    end
  end
end

return function(_env)
  return dispatcher.dispatch(_env)
end
