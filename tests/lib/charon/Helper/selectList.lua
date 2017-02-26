local Array  = require('charon.Array')
local helper = require('charon.Helper')
local test   = {}

test.should_return_html = function()
  local list   = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1)
  local html   = [[<select  id="cancel" name="cancel" style="width:250px"><option value="1" selected >value 1</option><option value="2"  >value 2</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_multiple_true = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { multiple = true } )
  local html   = [[<select  multiple="multiple" size="5"  id="cancel" name="cancel" style="width:250px"><option value="1" selected >value 1</option><option value="2"  >value 2</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_multiple_number = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { multiple = 7 } )
  local html   = [[<select  multiple="multiple" size="7"  id="cancel" name="cancel" style="width:250px"><option value="1" selected >value 1</option><option value="2"  >value 2</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_blank = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { blank = true } )
  local html   = [[<select  id="cancel" name="cancel" style="width:250px"><option value=""  ></option><option value="1" selected >value 1</option><option value="2"  >value 2</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_blank_description = function()
  local list = Array.new{
    { id = 1, description = 'value 1' },
    { id = 2, description = 'value 2' }
  }
  local result = helper:selectList("cancel", list, 'id', 'description', 1, { blank = 'SELECT' } )
  local html   = [[<select  id="cancel" name="cancel" style="width:250px"><option value=""  >SELECT</option><option value="1" selected >value 1</option><option value="2"  >value 2</option></select>]]
  assert( result == html, result )
end

return test
