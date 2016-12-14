-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Class = require "charon.oop.Class"
local template = require "template"

Controller = Class.new("Controller")
local helpers = {}

function Controller:render(params)
  return self['render_' .. params.output](self, params)
end

-------------------------------------------------------------------------------
-- RENDER HTML
-------------------------------------------------------------------------------

function Controller:resolvHelper()
  local prefix = "app.helpers."

  local helper = require("template.Helper")
  helper.__index = helper

  local file   = prefix .. "default"
  if os.exists(file:replace('.', '/') .. '.lua')  then
    local tmp = require(file)
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  end

  local file = prefix .. self.controller_name
  if os.exists(file:replace('.', '/') .. '.lua')  then
    local tmp = require(file)
    tmp.controller_path = self.controller_path
    tmp.controller_name = self.controller_name
    tmp.action_name     = self.action_name
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  else
    local tmp = {}
    tmp.controller_path = self.controller_path
    tmp.controller_name = self.controller_name
    tmp.action_name     = self.action_name
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  end
  helper.controller = self

  return helper
end

function Controller:helper()
  if helpers[self.controller_name] == nil or CHARON_ENV ~= 'production' then
    helpers[self.controller_name] = self:resolvHelper()
  end

  return helpers[self.controller_name]
end

function Controller:url(params)
  if type(params) == 'string' then
    return params
  end

  local dispatcher = require 'charon.dispatcher'
  local controller = params.controller or self.controller_name
  local action     = params.action or 'index'

  if dispatcher.prefix then
    controller = dispatcher.prefix .. '/' .. controller
  end

  params.action = nil
  params.controller = nil

  local result = '/' .. controller .. '/' .. action
  local query = true
  local open  = false
  for k, v in pairs(params) do
    if query then
      result = result .. '?'
      query  = false
    end
    if open then
      result = result .. '&'
    end
    result = result .. k .. '=' .. v
    open = true
  end

  return result
end

function Controller:redirect(params)
  local url  = self:url(params)
  local host = request.field("Host")
  local header = "Location: http://" .. host .. url
  return 302, {header}, nil
end

function Controller:render_js(params)
  local prefix = "app/views"
  local view   = nil
  local file   = nil

  if params.template == nil then
    if params.view == nil then
      file = prefix .. "/" .. self.controller_name .. "/" .. self.action_name .. ".js"
    else
      file = prefix .. "/" .. self.controller_name .. "/" .. params.view .. ".js"
    end
  else
    file = prefix .. "/" .. params.template .. ".js"
  end

  if self.layout then
    local flag, result = pcall(template.execute, file, self, self:helper())
    if flag then
      self._yield = result
    else
      self._yield = (file .. '\n\n' .. result .. '\n\n' .. template.debug(self._yield))
    end

    file = "app/views/layouts/" .. self.layout .. "_ajax.js"
  end

  local flag, result = pcall(template.execute, file, self, self:helper())
  if flag then
    return 200, {'Content-Type: text/javascript'}, result
  else
    return 500, {'Content-Type: text/plain'}, file .. '\n\n' .. result .. '\n\n' .. template.debug(file)
  end
end

function Controller:render_html(params)
  if params.value then
    return 200, {'Content-Type: text/html'}, params.value
  end

  local prefix = "app/views"
  local view   = nil
  local file   = nil

  if params.template == nil then
    if params.view == nil then
      file = prefix .. "/" .. self.controller_name .. "/" .. self.action_name .. ".html"
    else
      file = prefix .. "/" .. self.controller_name .. "/" .. params.view .. ".html"
    end
  else
    file = prefix .. "/" .. params.template .. ".html"
  end

  if self.layout then
    local flag, result
    if params.value then
      flag   = true
      result = params.value
    else
      flag, result = pcall(template.execute, file, self, self:helper())
    end
    if flag then
      self._yield = result
    else
      self._yield = (file .. '\n\n' .. result .. '\n\n' .. template.debug(self._yield))
    end

    if request.field("Accept") == "text/javascript" then
      file = "app/views/layouts/" .. self.layout .. ".js"
    else
      file = "app/views/layouts/" .. self.layout .. ".html"
    end
  end

  local flag, result = pcall(template.execute, file, self, self:helper())
  if flag then
    return 200, {'Content-Type: text/html'}, result
  else
    return 500, {'Content-Type: text/plain'}, file .. '\n\n' .. result .. '\n\n' .. template.debug(file)
  end
end

function Controller:partial(params)
  local prefix = "app/views"
  local view   = nil
  local file   = nil

  if params.template == nil then
    if params.view == nil then
      file = prefix .. "/" .. self.controller_name .. "/_" .. self.action_name .. ".html"
    else
      file = prefix .. "/" .. self.controller_name .. "/_" .. params.view .. ".html"
    end
  else
    file = prefix .. "/_" .. params.template
  end
  local context = params.context or self
  local flag, result = pcall(template.execute, file, context, self:helper())
  if flag then
    return result
  else
    error(file .. '\n\n' .. result .. '\n\n' .. template.debug(file))
  end
end

function Controller:yield()
--[[
  local flag, result = pcall(template.execute, self._yield, self, self:helper())
  if flag then
    return result
  else
    error(self._yield .. '\n\n' .. result .. '\n\n' .. template.debug(self._yield))
  end
]]
  return self._yield
end

-------------------------------------------------------------------------------
-- RENDER TEXT
-------------------------------------------------------------------------------

function Controller:render_text(params)
  local code = params.code or 200
  return code, {'Content-Type: text/plain'}, params.value
end

-------------------------------------------------------------------------------
-- RENDER JSON
-------------------------------------------------------------------------------

function Controller:render_json(params)
  local data = json.encode(params.value)
  if request.params().json_callback then
    data = string.format('%s(%s)', request.params().json_callback, data)
  end
  return 200, {'Content-Type: application/json; charset=UTF-8'}, data
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
