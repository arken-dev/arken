local Array  = require('arken.Array')
local helper = require('arken.Helper')
local test   = {}

test.should_return_html_with_options_default = function()
  local list   = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  helper.selectListOptions = { style = 'width:250px' }
  local result = helper:selectList("cancel", list, 'id', 'description', 1)
  local html   = '<select id="cancel" name="cancel" style="width:250px"><option value="1" selected >value 1</option><option value="2">value 2</option></select>'
  assert( result == html, result:replace(' ', '*') .. html:replace(' ', '*') )

  helper.selectListOptions = {}
  local result = helper:selectList("cancel", list, 'id', 'description', 1)
  local html   = '<select id="cancel" name="cancel"><option value="1" selected >value 1</option><option value="2">value 2</option></select>'
  assert( result == html, result:replace(' ', '*') .. html:replace(' ', '*') )
end

test.should_return_html_with_multiple_true = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { multiple = true } )
  local html   = '<select id="cancel" name="cancel" multiple="multiple" size="5"><option value="1" selected >value 1</option><option value="2">value 2</option></select>'
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

test.should_return_html_with_multiple_number = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { multiple = 7 } )
  local html   = '<select id="cancel" name="cancel" multiple="multiple" size="7"><option value="1" selected >value 1</option><option value="2">value 2</option></select>'
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

test.should_return_html_with_blank = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { blank = true } )
  local html   = '<select id="cancel" name="cancel"><option value=""></option><option value="1" selected >value 1</option><option value="2">value 2</option></select>'
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

test.should_return_html_with_blank_description = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { blank = 'SELECT' } )
  local html   = '<select id="cancel" name="cancel"><option value="">SELECT</option><option value="1" selected >value 1</option><option value="2">value 2</option></select>'
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

test['should return with many options'] = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local options = { blank = 'SELECT', onchange="alert('hello world')", style="width:150px", class='formDefault' }
  local result  = helper:selectList("cancel", list, 'id', 'description', 1, options )
  local html    = [[<select id="cancel" name="cancel" onchange="alert('hello world')" class="formDefault" style="width:150px"><option value="">SELECT</option><option value="1" selected >value 1</option><option value="2">value 2</option></select>]]
  assert( result == html, result:replace(' ', '*') .. '\n' .. html:replace(' ', '*') )
end

return test
