-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.
--
-- Helpers in charon platform follow similiar principle of Velocity Framework
-- http://stackoverflow.com/questions/4201142/rails-style-template-helpers-in-velocity
-- Examples:
-- <% local img = helper:img("pdf.png") -%>
-- <%= helper:floatField("total", {default = 0.0})
-- <%= helper:link{ text = 'cancel', url = {controller = "cancel"} } -%>
-- or
-- <a href="<%= helper:url{ action = 'cancel' } -%>">cancel</a>

local Form       = require "charon.Helper.FormHelper"
local url        = require "charon.net.url"
local toboolean  = require "charon.toboolean"
local dispatcher = require 'charon.dispatcher'
local Helper     = {}
local cache      = {}

Helper.publicPath = "public"

function Helper:hiddenField(field, value)
  value = value or ""
  local html = [[<input type="hidden" id="%s" name="%s" value=%q>]]
  return string.format(html, field:normalize(), field, value)
end

function Helper:textField(field, value, options)
  local value   = value   or ""
  local options = options or {}
  local style   = options.style or "width:250px"
  local html    = [[<input type="text" id="%s" name="%s" value=%q style=%q]]
  local html    = string.format(html, field:normalize(), field, value, style)

  if options.onblur then
    html = html .. string.format(" onblur=%q ", options.onblur)
  end

  if options.onfocus then
    html = html .. string.format(" onfocus=%q ", options.onfocus)
  end

  if options.onkeypress then
    html = html .. string.format(" onkeypress=%q ", options.onkeypress)
  end

  if options.onkeyup then
    html = html .. string.format(" onkeyup=%q ", options.onkeyup)
  end

  if options.disabled then
    html = html .. ' disabled'
  end

  return (html .. ' />')
end

function Helper:textArea(field, value, options)
  local value   = value   or ""
  local options = options or {}
  local style   = options.style or "width:250px;height:65px"
  local html    = [[<textarea id="%s" name="%s" style=%q]]
  local html    = string.format(html, field:normalize(), field, style)

  if options.onblur then
    html = html .. string.format(" onblur=%q ", options.onblur)
  end

  if options.onfocus then
    html = html .. string.format(" onfocus=%q ", options.onfocus)
  end

  if options.onkeypress then
    html = html .. string.format(" onkeypress=%q ", options.onkeypress)
  end

  if options.onkeyup then
    html = html .. string.format(" onkeyup=%q ", options.onkeyup)
  end

  return html .. string.format('>%s</textarea>', value)
end

function Helper:floatField(field, value, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '**.***.***,**')"
  options.style      = options.style      or "width:95px;text-align:right"
  return Helper:textField(field, value, options)
end

function Helper:intField(field, value, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '***.***.***')"
  options.style      = options.style      or "width:95px;text-align:right"
  return Helper:textField(field, value, options)
end

function Helper:dateField(field, value, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '**/**/****')"
  options.style      = options.style      or "width:80px;text-align:right"

  local html     = Helper:textField(field, value, options)
  local calendar = [[<script>jQuery.calendar('#%s')</script>]]

  return html .. string.format(calendar, field:normalize(), options)
end

function Helper:negativeLabel()
  return 'not'
end

function Helper:positiveLabel()
  return 'yes'
end

function Helper:boolField(field, value, options)
  local options = options or {}
  local html    = ""
  local label   = ""
  local checked = ""

  -- positive option
  label = options.positive or self:positiveLabel()
  if toboolean(value) == true then
    checked = "checked"
  else
    checked = ""
  end
  html = html .. string.format([[<input type="radio" name=%q value="true" %s > %s ]], field, checked, label)

  -- negative option
  label = options.negative or self:negativeLabel()
  if toboolean(value) == false then
    checked = "checked"
  else
    checked = ""
  end
  html = html .. string.format([[<input type="radio" name=%q value="false" %s > %s ]], field, checked, label)

  -- blank option
  if options.blank ~= nil then
    label = options.blank
    if value == "" or value == nil then
      checked = "checked"
    else
      checked = ""
    end
    html = html .. string.format([[<input type="radio" name=%q value="" %s > %s ]], field, checked, label)
  end

  return html
end

function Helper:selectList(field, list, field_value, field_description, value, options)
  options = options or {}
  local html     = "<select "
  local option   = "<option value=%q %s >%s</option>"
  local selected = ""
  local style    = options.style or "width:250px"

  if options.multiple then
    if options.multiple == true then
      options.multiple = 5
    end
    html = html .. string.format([[ multiple="multiple" size=%q ]], options.multiple)
  end

  html = html .. [[ id=%q name=%q style=%q>]]
  html = string.format(html, field:normalize(), field, style)
  if options.blank then
    local blank = ""
    if type(options.blank) == 'string' then
      blank = options.blank
    end
    html = html .. string.format(option, "", "", blank)
  end
  for row in list:each() do
    if row[field_value] == value then
      selected = 'selected'
    else
      selected = ''
    end
    html = html .. string.format(option, row[field_value], selected, row[field_description])
  end
  html = html .. "</select>"

  return html
end

function Helper:selectHash(field, list, value, options)
  options = options or {}
  local html     = "<select "
  local option   = "<option value=%q %s >%s</option>"
  local selected = ""

  if options.multiple then
    if options.multiple == true then
      options.multiple = 5
    end
    html = html .. string.format([[ multiple="multiple" size=%q ]], options.multiple)
  end

  local style = options.style or "width:250px;"
  html = html .. [[ id=%q name=%q style="%s">]]
  html = string.format(html, field:normalize(), field, style)
  if options.blank then
    local blank = ""
    if type(options.blank) == 'string' then
      blank = options.blank
    end
    html = html .. string.format(option, "", "", blank)
  end

  for k, v in pairs(list) do
    if k == value then
      selected = 'selected'
    else
      selected = ''
    end
    html = html .. string.format(option, k, selected, v)
  end
  html = html .. "</select>"

  return html
end

function Helper:fileTimestamp(file_name)
  if cache[file_name] == nil or CHARON_ENV ~= 'production' then
    if os.exists(Helper.publicPath .. file_name) then
      cache[file_name] = os.ctime(Helper.publicPath .. file_name)
    else
      cache[file_name] = 0
    end
  end
  return cache[file_name]
end

function Helper:javascript(file_name)
  local html = [[<script src="%s%s?%i" type="text/javascript"></script>]]
  file_name  = string.format("/javascripts/%s.js", file_name)
  return string.format(html, self.controller.prefix, file_name, self:fileTimestamp(file_name))
end

function Helper:stylesheet(file_name)
  local html = [[<link href="%s%s?%i" media="screen" rel="stylesheet" type="text/css" />]]
  file_name  = string.format("/stylesheets/%s.css", file_name)
  return string.format(html, self.controller.prefix, file_name, self:fileTimestamp(file_name))
end

function Helper:img(file_name, options)
  local options = options or {}
  local title   = options.title or file_name:left(file_name:lastIndexOf('.') - 1)
  local html    = [[<img src="%s%s?%i" title="%s" />]]
  file_name     = string.format("/images/%s", file_name)
  return string.format(html, self.controller.prefix, file_name, self:fileTimestamp(file_name), title)
end

function Helper:imgPath(file_name)
  local html    = '%s?%i'
  file_name     = string.format("%s/images/%s", self.controller.prefix, file_name)
  return string.format(html, file_name, self:fileTimestamp(file_name))
end

function Helper:formCreate(instance_or_name, name)
  local data
  if type(instance_or_name) == 'string' then
    data = self.controller[instance_or_name]
  else
    data = instance_or_name
  end
  return Form.new{ data = data, name = (name or 'form'), controller = self.controller, helper = self }
end

function Helper:url(params)

  if type(params) == 'string' then
    return params
  end

  local controller = params.controller or self.controllerName or 'index'
  local action     = params.action or 'index'

  if self.controller.prefix then
    controller = self.controller.prefix .. '/' .. controller
  end

  params.action = nil
  params.controller = nil

  local result = params.path or (controller .. '/' .. action)
  local query  = url.buildQuery(params)

  if #query > 0 then
    result = result .. '?' .. query
  end

  return result
end

function Helper:urlPerform()
  local action = self.actionName:gsub("Perform", "") .. "Perform"
  return self:url{ action = action }
end

function Helper:link(params)
  local html = [[<a href=%q]]

  html = string.format(html, self:url(params.url))

  if params.remote then
    html = html .. string.format(" data-remote=%q ", "true")
  end

  if params.after then
    html = html .. string.format(" data-after=%q ", params.after)
  end

  if params.click then
    html = html .. string.format(" onclick=%q ", params.click )
  end

  if params.update then
    html = html .. string.format(" data-update=%q ", params.update)
  end

  if params.confirm then
    html = html .. string.format(" data-confirm=%q ", params.confirm)
  end

  if params.with and params.remote then
    html = html .. string.format(" data-with=%q ", params.with)
  end

  if params.condition then
    html = html .. string.format(" data-condition=%q ", params.condition)
  end

  if params.style then
    html = html .. string.format(" style=%q ", params.style)
  end

  if params.class then
    html = html .. string.format(" class=%q ", params.class)
  end

  if params.title then
    html = html .. string.format(" title=%q ", params.title)
  end

  html = html .. '>'

  if params.img then
    html = html .. self:img(params.img, {title = params.title})
  else
    html = html .. params.text
  end

  html = html .. "</a>"
  return html
end

-------------------------------------------------------------------------------
-- AUTO COMPLETE
-------------------------------------------------------------------------------

function Helper:autoComplete(field, resource, options)
  local options = options or {}
  local field_value       = options.field_value or "value"
  local field_description = options.field_description or "label"
  local url     = self:url(resource)
  local style   = options.style or "width:250px"
  local html    = [[<input type="text" id="%s" name="%s" style=%q>]]
  local select  = options.select or ""
  html = string.format(html, field:normalize(), field, style)
  html = html .. string.format([[
  <script>
   jQuery(function() {
     jQuery( "#%s" ).autocomplete({
       source: function(request, response) {
         jQuery.ajax({
           url: '%s',
           dataType: 'jsonp',
           jsonp: 'json_callback',
           data: {
             field: %q,
             featureClass: 'P',
             style: 'full',
             limit: 10,
             busca: request.term
         },
         success: function( data ){
           response( jQuery.map( data, function( item ) {
             return {
               label: item.%s,
               value: item.%s,
               attributes: item
             }
           }));
         },
         error: function(params1, params2, params3) {
         //alert(params1+' - '+params2+' - '+params3);
         }
       })
     },
     select: function(event, ui) { %s; },
     minLength: 1
   });
  });
  </script>]], field:normalize(), url, field, field_description, field_value, select)

  return html
end

function Helper:dateFormat(value)
  return value
end

function Helper:dateTimeFormat(value)
  return value
end

function Helper:boolFormat(value, positive, negative)
  if toboolean(value) then
    return positive or 'yes'
  else
    return negative or 'no'
  end
end

-- nl2br
-- converte em uma string todos os \n para <br>
function Helper:nl2br(value)
  return tostring(value):replace('\n', '<br>')
end

return Helper
