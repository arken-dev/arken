local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html_not_checked = function()
  local list = { [1] =  'description' }
  local data = { id = '1' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectHash('id', list )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value="1"  >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_checked = function()
  local list = { [1] =  'description' }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectHash('id', list )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value="1" selected >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_multiple_true = function()
  local list = { [1] =  'description' }
  local data = { id = '1' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectHash('id', list, { multiple = true } )
  local html   = [[<select  multiple="multiple" size="5"  id="form_id" name="form[id]" style="width:250px;"><option value="1"  >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_multiple_number = function()
  local list = { [1] =  'description' }
  local data = { id = '1' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectHash('id', list, { multiple = 7 } )
  local html   = [[<select  multiple="multiple" size="7"  id="form_id" name="form[id]" style="width:250px;"><option value="1"  >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_blank = function()
  local list = { [1] =  'description' }
  local data = { id = '1' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectHash('id', list, { blank = true } )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value=""  ></option><option value="1"  >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_blank = function()
  local list = { [1] =  'description' }
  local data = { id = '1' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectHash('id', list, { blank = 'SELECT...' } )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value=""  >SELECT...</option><option value="1"  >description</option></select>]]
  assert( result == html, result )
end

return test
