require "Class"
template = require "template"

Controller = Class.new("Controller")

function Controller:render(params)
  return self['render_' .. params.output](self, params)
end

-------------------------------------------------------------------------------
-- RENDER HTML
-------------------------------------------------------------------------------

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

  return 200, {'Content-Type: text/html'}, template.execute(file, self)
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
  return template.execute(file, context)
end

function Controller:yield()
  return template.execute(self._yield, self)
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
