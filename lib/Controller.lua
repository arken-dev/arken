require "Class"
template = require "template"

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

  local helper = require("template.helper")
  helper.__index = helper

  local file   = prefix .. "default"
  if QFile.exists(file:replace('.', '/') .. '.lua')  then
    local tmp = require(file)
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  end

  local file = prefix .. self.controller_name
  if QFile.exists(file:replace('.', '/') .. '.lua')  then
    local tmp = require(file)
    tmp.__index = tmp
    setmetatable(tmp, helper)
    helper = tmp
  end

  return helper
end

function Controller:helper()
  if helpers[self.controller_name] == nil or OBERON_ENV ~= 'production' then
    helpers[self.controller_name] = self:resolvHelper()
  end

  return helpers[self.controller_name]
end

function Controller:url(params)
  local result = '/' .. self.controller_name .. '/'
  if params.action then
    result = result .. params.action
  else
    result = result .. 'index'
  end

  return result
end

function Controller:redirect(params)
  local url  = self:url(params)
  local host = request.field("Host")
  return 301, ("Location: http://" .. host .. url)
end

function Controller:render_html(params)
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
    file = prefix .. "/" .. params.template
  end

  if self.layout then
    self._yield = file
    file = "app/views/layouts/" .. self.layout .. ".html"
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
  local flag, result = pcall(template.execute, self._yield, self, self:helper())
  if flag then
    return result
  else
    error(self._yield .. '\n\n' .. result .. '\n\n' .. template.debug(self._yield))
  end
end

-------------------------------------------------------------------------------
-- RENDER TEXT
-------------------------------------------------------------------------------

function Controller:render_text(params)
  return 200, {['Content-Type'] = 'text/plain'}, params.value
end

-------------------------------------------------------------------------------
-- EXECUTE
-------------------------------------------------------------------------------

function Object:execute(method, params)
  self:prepare(params)
  self:validate(params)
  self:before(params)
  local code, headers, data = self[method](self, params)
  self:after(params)
  return code, headers, data
end

return Controller
