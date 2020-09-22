local helper = require('arken.Helper')
local test   = {}

test.should_return_html = function()
  local result = helper:boolField("cancel", "")
  local html   = [[<input type="radio" name="cancel" value="true"  > yes <input type="radio" name="cancel" value="false"  > not ]]
  assert( result == html, result )
end

test.should_return_html_with_negative_checked = function()
  local result = helper:boolField("cancel", false)
  local html   = [[<input type="radio" name="cancel" value="true"  > yes <input type="radio" name="cancel" value="false" checked > not ]]
  assert( result == html, result )
end

test.should_return_html_with_negative_string_checked = function()
  local result = helper:boolField("cancel", "false")
  local html   = [[<input type="radio" name="cancel" value="true"  > yes <input type="radio" name="cancel" value="false" checked > not ]]
  assert( result == html, result )
end

test.should_return_html_with_positive_checked = function()
  local result = helper:boolField("cancel", true)
  local html   = [[<input type="radio" name="cancel" value="true" checked > yes <input type="radio" name="cancel" value="false"  > not ]]
  assert( result == html, result )
end

test.should_return_html_with_positive_string_checked = function()
  local result = helper:boolField("cancel", "true")
  local html   = [[<input type="radio" name="cancel" value="true" checked > yes <input type="radio" name="cancel" value="false"  > not ]]
  assert( result == html, result )
end

test.should_return_html_with_blank_option = function()
  local result = helper:boolField("cancel", "true", { blank = 'boot' })
  local html   = [[<input type="radio" name="cancel" value="true" checked > yes <input type="radio" name="cancel" value="false"  > not <input type="radio" name="cancel" value=""  > boot ]]
  assert( result == html, result )
end

test.should_return_html_with_blank_checked_option = function()
  local result = helper:boolField("cancel", "", { blank = 'boot' })
  local html   = [[<input type="radio" name="cancel" value="true"  > yes <input type="radio" name="cancel" value="false"  > not <input type="radio" name="cancel" value="" checked > boot ]]
  assert( result == html, result )
end

test.should_return_html_with_nil_checked_option = function()
  local result = helper:boolField("cancel", nil, { blank = 'boot' })
  local html   = [[<input type="radio" name="cancel" value="true"  > yes <input type="radio" name="cancel" value="false"  > not <input type="radio" name="cancel" value="" checked > boot ]]
  assert( result == html, result )
end

return test
