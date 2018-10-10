-- Copyright 2016 The Charon Platform Authors.
-- All rights reserved.
-- Use of this source code is governed by a BSD-style
-- license that can be found in the LICENSE file.

local Class     = require 'charon.oop.Class'
local url       = require 'charon.net.url'
local toboolean = require 'charon.toboolean'

FormHelper = Class.new("FormHelper")

function FormHelper:url(params)

  if type(params) == 'string' then
    return params
  end

  local dispatcher = require 'charon.dispatcher'
  local controller = params.controller or self.controller.controllerName or 'index'
  local action     = params.action or self.controller.actionName or 'index'

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

function FormHelper:urlPerform()
  local action = self.controller.actionName:gsub("Perform", "") .. "Perform"
  return self:url{ action = action }
end

function FormHelper:buildId(field)
  return (self.name .. '_' .. field)
end

function FormHelper:buildName(field)
  return (self.name .. '[' .. field .. ']')
end

function FormHelper:buildValue(field)
  local value
  if self.data then
    if self.data.read then
      value = self.data['read'](self.data, field)
    else
      value = self.data[field]
    end
  end
  if value == nil then
    return ''
  else
    return value
  end
end

function FormHelper:hiddenField(field)
  local html = [[<input type="hidden" id="%s" name="%s" value=%q>]]
  return string.format(html, self:buildId(field), self:buildName(field), self:buildValue(field))
end

function FormHelper:textField(field, options)
  options       = options or {}
  local value   = options.value or self:buildValue(field)
  local options = options or {}
  local style   = options.style or "width:250px"
  local html    = [[<input type="text" id="%s" name="%s" value=%q style=%q]]
  local html    = string.format(html, self:buildId(field), self:buildName(field), value, style)

  if options.onblur then
    html = html .. string.format(" onblur=%q", options.onblur)
  end

  if options.onfocus then
    html = html .. string.format(" onfocus=%q", options.onfocus)
  end

  if options.onkeypress then
    html = html .. string.format(" onkeypress=%q", options.onkeypress)
  end

  if options.onkeyup then
    html = html .. string.format(" onkeyup=%q", options.onkeyup)
  end

  return (html .. ' />')
end

function FormHelper:textArea(field, options)
  options       = options or {}
  local value   = options.value or self:buildValue(field)
  local options = options or {}
  local style   = options.style or "width:250px;height:65px"
  local html    = [[<textarea id=%q name=%q style=%q]]
  local html    = string.format(html, self:buildId(field), self:buildName(field), style)

  if options.onblur then
    html = html .. string.format(" onblur=%q", options.onblur)
  end

  if options.onfocus then
    html = html .. string.format(" onfocus=%q", options.onfocus)
  end

  if options.onkeypress then
    html = html .. string.format(" onkeypress=%q", options.onkeypress)
  end

  if options.onkeyup then
    html = html .. string.format(" onkeyup=%q", options.onkeyup)
  end

  return html .. string.format('>%s</textarea>', value)
end


function FormHelper:floatField(field, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '**.***.***,**')"
  options.style      = options.style      or "width:95px;text-align:right"
  local decimal      = options.decimal    or 2
  local separator    = options.separator  or ','
  local thousands    = options.thousands  or '.'
  local value        = options.value      or self:buildValue(field)

  if type(value) == 'string' and value:contains(',') then
    value = tonumber( value:replace('.', ''):replace(',', '.') )
  else
    value = tonumber(value)
  end

  if value == nil then
    options.value = ""
  else
    options.value = math.format(value, decimal, separator, thousands)
  end

  return self:textField(field, options)
end

function FormHelper:intField(field, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '***.***.***')"
  options.style      = options.style      or "width:95px;text-align:right"
  local value        = options.value      or self:buildValue(field)
  local thousands    = options.thousands  or '.'

  if type(value) == 'string' and value:contains(',') then
    value = tonumber( value:replace('.', ''):replace(',', '.') )
  else
    value = tonumber(value)
  end

  if value == nil then
    options.value = ""
  else
--    options.value = math.format(value, decimal, separator, thousands)
  end

  return self:textField(field, options)
end

function FormHelper:dateField(field, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '**/**/****')"
  options.style      = options.style      or "width:80px;text-align:right"

  local value = tostring( self:buildValue(field) )

  if value:sub(5, 5) == '/' then
    local list = value:split('/')
    value = tostring(list:at(3)) .. '/' .. tostring(list:at(2)) .. '/' .. tostring(list:at(1))
    options.value = value
  end

  if value:sub(5, 5) == '-' then
    local list = value:split('-')
    value = tostring(list:at(3)) .. '-' .. tostring(list:at(2)) .. '-' .. tostring(list:at(1))
    options.value = value
  end

  local html     = self:textField(field, options)
  local calendar = [[<script>jQuery.calendar('#%s')</script>]]

  return html .. string.format(calendar, self:buildId(field), options)
end

function FormHelper:submitSave()
  return [[<input type="image" src="/images/icons/botao_gravar.png?1453303287" style="border: 0px" >]]
end

function FormHelper:submitCancel()
  return self.helper:link{ img = 'icons/botao_cancelar.png', url = self:url{ action = 'cancel' }, remote = true }
end

--------------------------------------------------------------------------------
-- SELECT LIST
--------------------------------------------------------------------------------

function FormHelper:selectList(field, list, field_value, field_description, options)
  options = options or {}
  local html     = "<select "
  local option   = "<option value=%q %s >%s</option>"
  local selected = ""
  local value    = self:buildValue(field)
  local style    = options.style or 'width:250px;'

  if options.multiple then
    if options.multiple == true then
      options.multiple = 5
    end
    html = html .. string.format([[ multiple="multiple" size=%q ]], options.multiple)
  end

  html = html .. [[ id=%q name=%q style=%q>]]
  html = string.format(html, self:buildId(field), self:buildName(field), style)

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

--------------------------------------------------------------------------------
-- SELECT HASH
--------------------------------------------------------------------------------

function FormHelper:selectHash(field, list, options)
  options = options or {}
  local html     = "<select "
  local option   = "<option value=%q %s >%s</option>"
  local selected = ""
  local value    = self:buildValue(field)
  local style    = options.style or "width:250px"

  if options.multiple then
    if options.multiple == true then
      options.multiple = 5
    end
    html = html .. string.format([[ multiple="multiple" size=%q ]], options.multiple)
  end

  html = html .. [[ id=%q name=%q style=%q>]]
  html = string.format(html, self:buildId(field), self:buildName(field), style)

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

-------------------------------------------------------------------------------
-- AUTO COMPLETE
-------------------------------------------------------------------------------

function FormHelper:autoComplete(field, resource, options)
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
  </script>]], field:normalize(), url, field_description, field_value, select)

  return html
end

-------------------------------------------------------------------------------
-- BOOL FIELD
-------------------------------------------------------------------------------

FormHelper.negativeLabel = function()
  return 'not'
end

FormHelper.positiveLabel = function ()
  return 'yes'
end

function FormHelper:boolField(field, options)
  local value   = self:buildValue(field)
  local options = options or {}
  local html    = ""
  local label   = ""
  local checked = ""
  local input   = ""

  -- positive option
  label = options.positive or FormHelper.positiveLabel()
  if toboolean(value) == true then
    checked = "checked"
  else
    checked = ""
  end
  input = [[<input type="radio" name=%q value="true" %s > %s ]]
  html  = html .. string.format(input, self:buildName(field), checked, label)

  -- negative option
  label = options.negative or FormHelper.negativeLabel()
  if toboolean(value) == false then
    checked = "checked"
  else
    checked = ""
  end
  input = [[<input type="radio" name=%q value="false" %s > %s ]]
  html = html .. string.format(input, self:buildName(field), checked, label)

  -- blank option
  if options.blank ~= nil then
    label = options.blank
    if value == "" or value == nil then
      checked = "checked"
    else
      checked = ""
    end
    input = [[<input type="radio" name=%q value="" %s > %s ]]
    html = html .. string.format(input, self:buildName(field), checked, label)
  end

  return html
end

function FormHelper:toggleField(field, options)
  local options = options or {}
  local label   = options.label or 'yes'
  local value   = self:buildValue(field)
  local html    = [[<input type="checkbox" name=%q value="true" %s]]
  local checked = ""

  if options.title then
    html = html .. string.format(" title=%q", options.title)
  end

  if toboolean(value) == true then
    checked = "checked"
  end

  return string.format(html, self:buildName(field), checked) .. ' />' .. label
end

return FormHelper
