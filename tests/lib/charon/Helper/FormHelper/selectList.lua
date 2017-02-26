local Array      = require('charon.Array')
local FormHelper = require('charon.Helper.FormHelper')
local test = {}

test.should_return_html_not_checked = function()
  local list = Array.new{ { id = 1, name = 'description' } }
  local data = { id = '1' }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name' )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value="1"  >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_checked = function()
  local list = Array.new{ { id = 1, name = 'description' } }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name' )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value="1" selected >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_multiple_list_and_checked = function()
  local list = Array.new{
    { id = 1, name = 'John' },
    { id = 2, name = 'Suse' },
    { id = 3, name = 'Rute' }
  }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name' )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value="1" selected >John</option><option value="2"  >Suse</option><option value="3"  >Rute</option></select>]]
  assert( result == html, result )
end


test.should_return_html_multiple_true = function()
  local list = Array.new{ { id = 1, name = 'description' } }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name', { multiple = true } )
  local html   = [[<select  multiple="multiple" size="5"  id="form_id" name="form[id]" style="width:250px;"><option value="1" selected >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_multiple = function()
  local list = Array.new{ { id = 1, name = 'description' } }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name', { multiple = 7 } )
  local html   = [[<select  multiple="multiple" size="7"  id="form_id" name="form[id]" style="width:250px;"><option value="1" selected >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_blank_true = function()
  local list = Array.new{ { id = 1, name = 'description' } }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name', { blank = true } )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value=""  ></option><option value="1" selected >description</option></select>]]
  assert( result == html, result )
end

test.should_return_html_blank_value = function()
  local list = Array.new{ { id = 1, name = 'description' } }
  local data = { id = 1 }
  local controller = {controller_name = 'form', action_name = 'save'}
  local form = FormHelper.new{ name = 'form', controller = controller, data = data }
  local result = form:selectList('id', list, 'id', 'name', { blank = 'SELECT OPTION' } )
  local html   = [[<select  id="form_id" name="form[id]" style="width:250px;"><option value=""  >SELECT OPTION</option><option value="1" selected >description</option></select>]]
  assert( result == html, result )
end

return test
