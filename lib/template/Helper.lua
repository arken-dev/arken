-- http://stackoverflow.com/questions/4201142/rails-style-template-helpers-in-velocity
-- http://framework.zend.com/manual/current/en/modules/zend.view.helpers.html
-- http://framework.zend.com/manual/current/en/modules/zend.view.helpers.url.html

--[[
<?php echo $this->formText('email', 'you@example.com', array('size' => 32)) ?>
<%=        helper.formText('email', 'you@example.com', array('size' => 32)) %>


<% local img = helper.img("teste") -%>
<%= helper.link(img, {url = "teste"}) -%>
<%= helper.link(helper.img("icons/detalhes.png") , {url = "teste"}) -%>


<%= helper.link{img = "icons/teste.png", url = "teste"} -%>


<% local form = helper.form("pedido") -%>
<%= form.floatField("total", {default = 0.0})
<a href="<%= helper.url{ action = 'concluir' } -%>" data-remote='true'><%= helper.image("icone/detalhes") -%></a>
]]

require "template.FormHelper"

local url   = require 'url'
local M     = {}
local cache = {}

function M:hiddenField(field, value)
  value = value or ""
  local html = [[<input type="hidden" id="%s" name="%s" value=%q>]]
  return string.format(html, field:normalize(), field, value)
end

function M:textField(field, value, options)
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

  return (html .. ' />')
end

function M:textArea(field, value, options)
  local value   = value   or ""
  local options = options or {}
  local style   = options.style or "width:250px;height:65px"
  local html    = [[<textarea id="%s" name="%s" style=%q>%s]]
  local html    = string.format(html, field:normalize(), field, style, value)

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

  return (html .. '</textarea>')
end

function M:floatField(field, value, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '**.***.***,**')"
  options.style      = options.style      or "width:95px;text-align:right"
  return M:textField(field, value, options)
end

function M:intField(field, value, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '***.***.***')"
  options.style      = options.style      or "width:95px;text-align:right"
  return M:textField(field, value, options)
end

function M:dateField(field, value, options)
  options            = options            or {}
  options.onblur     = options.onblur     or "inputBlur( this )"
  options.onfocus    = options.onfocus    or "inputFocus( this )"
  options.onkeypress = options.onkeypress or "inputPress( this, event )"
  options.onkeyup    = options.onkeyup    or "format_invert(this, '**/**/****')"
  options.style      = options.style      or "width:80px;text-align:right"

  local html     = M:textField(field, value, options)
  local calendar = [[<script>jQuery.calendar('#%s')</script>]]

  return html .. string.format(calendar, field, options)
end

function M:boolField(field, value, options)
  local options = options or {}
  local html    = ""
  local label   = ""
  local checked = ""

  -- positive option
  label = options.positive or " sim"
  if toboolean(value) == true then
    checked = "checked"
  else
    checked = ""
  end
  html = html .. string.format([[<input type="radio" name=%q value="true" %s > %s ]], field, checked, label)

  -- negative option
  label = options.negative or " não"
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

function M:selectList(field, list, field_value, field_description, value, options)
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
  for i = 1, #list do
    local row = list[i]
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

function M:selectHash(field, list, value, options)
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

  html = html .. [[ id=%q name=%q style="width:250px;">]]
  html = string.format(html, field:normalize(), field)
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
    html = html .. string.format(option, tostring(k), selected, tostring(v))
  end
  html = html .. "</select>"

  return html
end

function M:submitSend()
  return [[<input type="image" src="/images/icons/botao_enviar.png?1453303287" style="border: 0px" >]]
end

function M:submitSave()
  return [[<input type="image" src="/images/icons/botao_gravar.png?1453303287" style="border: 0px" >]]
end

function M:submitCancel()
  return self:link{ img = 'icons/botao_cancelar.png', url = { action = 'cancel' }, remote = true }
end

function M:fileTimestamp(file_name)
  if cache[file_name] == nil or CHARON_ENV ~= 'production' then
    if os.exists('public' .. file_name) then
      cache[file_name] = os.ctime('public' .. file_name)
    else
      cache[file_name] = 0
    end
  end
  return cache[file_name]
end

function M:javascriptSrc(file_name)
  local html = [[<script src="/app%s?%i" type="text/javascript"></script>]]
  file_name  = string.format("/javascripts/%s.js", file_name)
  return string.format(html, file_name, self:fileTimestamp(file_name))
end

function M:stylesheetSrc(file_name)
  local html = [[<link href="/app%s?%i" media="screen" rel="stylesheet" type="text/css" />]]
  file_name  = string.format("/stylesheets/%s.css", file_name)
  return string.format(html, file_name, self:fileTimestamp(file_name))
end

function M:img(file_name, options)
  local options = options or {}
  local title   = options.title or ""
  local html    = [[<img src="%s?%i" title="%s" />]]
  file_name     = string.format("/images/%s", file_name)
  return string.format(html, file_name, self:fileTimestamp(file_name), title)
end

function M:imgPath(file_name)
  local html    = '%s?%i'
  file_name     = string.format("/images/%s", file_name)
  return string.format(html, file_name, self:fileTimestamp(file_name))
end

function M:formCreate(instance_or_name, name)
  local data
  if type(instance_or_name) == 'string' then
    data = self.controller[instance_or_name]
  else
    data = instance_or_name
  end
  return FormHelper.new{ data = data, name = (name or 'form'), controller = self.controller }
end

function M:url(params)

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

  local result = params.path or ('/' .. controller .. '/' .. action)
  local query  = url.buildQuery(params)

  if #query > 0 then
    result = result .. '?' .. query
  end

  return result
end

function M:urlPerform()
  local action = self.action_name:gsub("Perform", "") .. "Perform"
  return self:url{ action = action }
end

function M:link(params)
  local html = [[<a href=%q]]

  if params.modal then
    local onclick = "modal_open"

    if params.condition == nil and params.with then
      onclick = " checkArg() && " .. onclick
    end

    if params.condition then
      onclick = params.condition .. " && " .. onclick
    end

    if params.confirm then
      onclick = string.format("window.confirm(%q) && ", params.confirm) .. onclick
    end

    if params.with then
      html = string.format(html, "#") .. [[ onclick='%s(%q+jQuery.param(%s)); return false;' ]]
      local url = self:url(params.url)
      if url:contains("?") then
        url = url .. "&"
      else
        url = url .. "?"
      end
      html = string.format(html, onclick, url, params.with)
    else
      html = string.format(html, "#") .. [[ onclick='modal_open(%q); return false;' ]]
      html = string.format(html, self:url(params.url))
    end

  else

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
      if params.condition == nil then
        html = html .. string.format(" data-condition=%q ", "checkArg()")
      end
    end

    if params.condition then
      html = html .. string.format(" data-condition=%q ", params.condition)
    end

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

function M:autoComplete(field, resource, options)
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

-------------------------------------------------------------------------------
-- AUTO LIST
-------------------------------------------------------------------------------

function M:autoList(field, resource)
  local controller
  local action
  if type(resource) == 'table' then
    controller = resource.controller
    action     = resource.action or 'index'
  else
    controller = resource
  end
  local func  = string.format([[AutoList.select(%q, %q, %q, ui);]], field, (field .. "_id"), (field .. "_id"))
  local input = self:autoComplete( 'autolist[' .. field .. ']',
    { controller = controller, action = action },
    { ['select'] = func }
  )

  local link  = self:link {
    img    = 'icons/refresh.gif',
    url    = '#',
    click  = string.format([[AutoList.list('%s', '%s', '%s', '%s');]],
      field, (field .. "_id"), (field .. "_id"),
      self:url({ controller = controller, action = 'select', field = (field .. "_id") })
    )
  }

  local html = [[
    <table cellspacing='0' cellpadding='0'>
      <tr>
        <td width='250'>
          %s
          <div id='autolist_%s_container' style='width:250px;display:none'></div>
        </td>
        <td>&nbsp;</td>
        <td>%s</td>
      </tr>
    </table>
  ]]

  return string.format(html, input, field, link)
end

function M:slice(value, start, stop)

  if value==nil  then
    return ""
  end

  return string.sub(tostring(value), start, stop)
end

function M:dateFormat(value)
  if value==nil then
    return ""
  end

  local dia = M:slice(value, 9, 10)
  local mes = M:slice(value, 6, 7)
  local ano = M:slice(value, 0, 4)
  return dia .. "/" .. mes .. "/" .. ano
end

function M:dateTimeFormat(value)
  return value
end


function M:trucante(value, size)
  return value
end

function M:boolFormat(value, positive, negative)
  if toboolean(value) then
    return positive or 'sim'
  else
    return negative or 'não'
  end
end

function M:dateIntervalo(field)
  return M:dateField(field .. "_inicio") .. "&nbsp;à&nbsp;" .. M:dateField(field .. "_fim")
end

-- nl2br
-- converte em uma string todos os \n para <br>
function M:nl2br(value)
  return tostring(value):swap('\n', '<br>')
end

function M:toNumber(value)
  if value == nil then
    return 0
  else
    return tonumber(value)
  end
end

return M
