local helper = require('arken.Helper')
local test   = {}

test.should_return_html = function()
  local list   = {
    [1] = 'value 1',
    [2] = 'value 2'
  }
  local result = helper:selectHash("cancel", list)
  local html   = [[<select  id="cancel" name="cancel" style="width:250px;"><option value="2"  >value 2</option><option value="1"  >value 1</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_multiple_true = function()
  local list   = {
    [1] = 'value 1',
    [2] = 'value 2'
  }
  local result = helper:selectHash("cancel", list, nil, { multiple = true })
  local html   = [[<select  multiple="multiple" size="5"  id="cancel" name="cancel" style="width:250px;"><option value="2"  >value 2</option><option value="1"  >value 1</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_multiple_number = function()
  local list   = {
    [1] = 'value 1',
    [2] = 'value 2'
  }
  local result = helper:selectHash("cancel", list, nil, { multiple = 7 })
  local html   = [[<select  multiple="multiple" size="7"  id="cancel" name="cancel" style="width:250px;"><option value="2"  >value 2</option><option value="1"  >value 1</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_selected = function()
  local list   = {
    [1] = 'value 1',
    [2] = 'value 2'
  }
  local result = helper:selectHash("cancel", list, 1)
  local html   = [[<select  id="cancel" name="cancel" style="width:250px;"><option value="2"  >value 2</option><option value="1" selected >value 1</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_blank = function()
  local list   = {
    [1] = 'value 1',
    [2] = 'value 2'
  }
  local result = helper:selectHash("cancel", list, 1, { blank = true })
  local html   = [[<select  id="cancel" name="cancel" style="width:250px;"><option value=""  ></option><option value="2"  >value 2</option><option value="1" selected >value 1</option></select>]]
  assert( result == html, result )
end

test.should_return_html_with_blank_option = function()
  local list   = {
    [1] = 'value 1',
    [2] = 'value 2'
  }
  local result = helper:selectHash("cancel", list, 1, { blank = 'SELECT' })
  local html   = [[<select  id="cancel" name="cancel" style="width:250px;"><option value=""  >SELECT</option><option value="2"  >value 2</option><option value="1" selected >value 1</option></select>]]
  assert( result == html, result )
end

return test
