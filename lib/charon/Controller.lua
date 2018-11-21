-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Class      = require "charon.oop.Class"
local template   = require "charon.template"
local url        = require "charon.net.url"
local json       = require "charon.json"

local Controller = Class.new("Controller", "charon.net.HttpRequest")

Controller.prefix        = ""
Controller.prefixHelpers = "app.helpers"
Controller.prefixViews   = "app/views"

local helpers = {}

-------------------------------------------------------------------------------
-- RENDER
-------------------------------------------------------------------------------

function Controller:render(params)
  return self['render_' .. params.output](self, params)
end

-------------------------------------------------------------------------------
-- SEND
-------------------------------------------------------------------------------

function Controller:send(params)
  local code, headers, body = self['render_' .. params.output](self, params)
  if code == 200 then
    local filename = params.filename or (self.className:underscore():replace("_", "-") .. "." .. params.output)
    table.insert( headers, "Content-Disposition: attachment;filename=" .. filename .. "; charset=UTF-8" )
  end
  return code, headers, body
end

-------------------------------------------------------------------------------
-- RENDER HTML
-------------------------------------------------------------------------------

function Controller:resolvHelper()

  local helper   = require("charon.Helper")
  helper.__index = helper

  local file   = self.prefixHelpers .. ".default"
  if os.exists(file:replace('.', '/') .. '.lua')  then
    local tmp   = require(file)
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  end

  local file = self.prefixHelpers .. "." .. self.controllerName
  if os.exists(file:replace('.', '/') .. '.lua')  then
    local tmp = require(file)
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  else
    local tmp = {}
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  end

  return helper
end

function Controller:helper()
  if helpers[self.controllerName] == nil or CHARON_ENV ~= 'production' then
    helpers[self.controllerName] = self:resolvHelper()
  end
  local tmp = helpers[self.controllerName]

  tmp.controllerPath = self.controllerPath
  tmp.controllerName = self.controllerName
  tmp.actionName     = self.actionName
  tmp.controller     = self

  return tmp
end

function Controller:url(params)
  return self:helper():url(params)
end

function Controller:redirect(params)
  local url    = self:url(params)
  local host   = self:env():field("Host")
  local header = "Location: http://" .. host .. url
  return 302, {header}, nil
end

function Controller:render_js(params)
  local fileName = nil

  if params.template == nil then
    if params.view == nil then
      fileName = self.prefixViews .. "/" .. self.controllerName .. "/" .. self.actionName .. ".js"
    else
      fileName = self.prefixViews .. "/" .. self.controllerName .. "/" .. params.view .. ".js"
    end
  else
    fileName = self.prefixViews .. "/" .. params.template .. ".js"
  end

  if self.layout then
    local flag, result = pcall(template.execute, fileName, self, self:helper(), CHARON_ENV ~= 'production')
    if flag then
      self._yield = result
    else
      self._yield = (fileName .. '\n\n' .. result .. '\n\n' .. template.debug(fileName))
    end
    fileName = self.prefixViews .. "/layouts/" .. self.layout .. ".js"
  end

  local flag, result = pcall(template.execute, fileName, self, self:helper(), CHARON_ENV ~= 'production')
  if flag then
    return 200, {'Content-Type: text/javascript'}, result
  else
    return 500, {'Content-Type: text/plain'}, fileName .. '\n\n' .. result .. '\n\n' .. template.debug(fileName)
  end
end

function Controller:render_html(params)

  if params.value and not params.layout then
    return 200, {'Content-Type: text/html'}, params.value
  end

  local file   = nil

  if params.template == nil then
    if params.view == nil then
      file = self.prefixViews .. "/" .. self.controllerName .. "/" .. self.actionName .. ".html"
    else
      file = self.prefixViews .. "/" .. self.controllerName .. "/" .. params.view .. ".html"
    end
  else
    file = self.prefixViews .. "/" .. params.template .. ".html"
  end

  if self.layout and params.layout ~= false then
    local flag, result
    if params.value then
      flag   = true
      result = params.value
    else
      flag, result = pcall(template.execute, file, self, self:helper(), CHARON_ENV ~= 'production')
    end
    if flag then
      self._yield = result
    else
      return 500, {'Content-Type: text/plain'}, file .. '\n\n' .. result .. '\n\n' .. template.debug(file)
    end

    file = self.prefixViews .. "/layouts/" .. self.layout .. ".html"
  end

  local flag, result = pcall(template.execute, file, self, self:helper(), CHARON_ENV ~= 'production')
  if flag then
    return 200, {'Content-Type: text/html'}, result
  else
    return 500, {'Content-Type: text/plain'}, file .. '\n\n' .. result .. '\n\n' .. template.debug(file)
  end
end

function Controller:renderTemplate(params, ext)
  local file   = nil

  if params.template == nil then
    if params.view == nil then
      file = self.prefixViews .. "/" .. self.controllerName .. "/" .. self.actionName .. "." .. ext
    else
      file = self.prefixViews .. "/" .. self.controllerName .. "/" .. params.view .. "." .. ext
    end
  else
    file = self.prefixViews .. "/" .. params.template .. "." .. ext
  end

  if self.layout and params.layout ~= false then
    local flag, result
    if params.value then
      flag   = true
      result = params.value
    else
      flag, result = pcall(template.execute, file, self, self:helper(), CHARON_ENV ~= 'production')
    end
    if flag then
      self._yield = result
    else
      return 500, {'Content-Type: text/plain'}, file .. '\n\n' .. result .. '\n\n' .. template.debug(file)
    end

    file = self.prefixViews .. "/layouts/" .. self.layout .. "." .. ext
  end

  local flag, result = pcall(template.execute, file, self, self:helper(), CHARON_ENV ~= 'production')
  if flag then
    return true, result
  else
    return false, file .. '\n\n' .. result .. '\n\n' .. template.debug(file)
  end
end


function Controller:partial(params)
  params = params or {}
  local file   = nil

  if params.view == nil then
    file = self.prefixViews .. "/" .. self.controllerName .. "/_" .. self.actionName .. ".html"
  else
    file = self.prefixViews .. "/" .. self.controllerName .. "/_" .. params.view .. ".html"
  end

  local context = params.context or self
  local flag, result = pcall(template.execute, file, self, self:helper(), CHARON_ENV ~= 'production')
  if flag then
    return result
  else
    error(file .. '\n\n' .. result .. '\n\n' .. template.debug(file))
  end
end

function Controller:yield()
  return self._yield
end

-------------------------------------------------------------------------------
-- RENDER TEXT
-------------------------------------------------------------------------------

function Controller:render_text(params)
  local code    = params.code or 200
  local body    = params.value
  local headers = {'Content-Type: text/plain'}
  local flag    = true
  if body == nil then
    flag, body = self:renderTemplate(params, 'txt')
    if not flag then
      code = 500
    end
  end
  return code, headers, body
end

-------------------------------------------------------------------------------
-- RENDER JSON
-------------------------------------------------------------------------------

function Controller:render_json(params)
  local code = params.code or 200
  local data = json.encode(params.value)
  if self:params().json_callback then
    data = string.format('%s(%s)', self:params().json_callback, data)
  end
  return code, {'Content-Type: application/json; charset=UTF-8'}, data
end

-------------------------------------------------------------------------------
-- EXECUTE
-------------------------------------------------------------------------------

function Controller:execute(method, params)
  local code, headers, data
  self:prepare(params)
  code, headers, data = self:validate(params)
  if code ~= nil then
    return code, headers, data
  end
  self:before(params)
  code, headers, data = self[method](self, params)
  self:after(params)
  self:response(headers)
  return code, headers, data
end

function Controller:pexecute(method, params)
  local status, code_or_errors, headers, data = pcall(self.execute, self, method, params)
  if status then
    return code_or_errors, headers, data
  else
    return self:rescue(code_or_errors)
  end
end

function Controller:rescue(errors)
  local body = ""
  if type(errors) == 'string' then
    body = errors
  end
  if type(errors) == 'table' then
    for key, value in pairs(errors) do
      body = body .. value .. '\n'
    end
  end
  return 500, {}, body
end

return Controller
