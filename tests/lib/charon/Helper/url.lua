local helper = require('charon.Helper')
local test   = {}

test.should_return_html_index_controller_and_index_action = function()
  local result = helper:url{ action = 'index' }
  local html   = [[/index/index]]
  assert( result == html, result )
end

test.should_return_html_named_controller_and_index_action = function()
  local result = helper:url{ controller = 'users' }
  local html   = [[/users/index]]
  assert( result == html, result )
end

test.should_return_html_index_controller_and_named_action = function()
  local result = helper:url{ action = 'users' }
  local html   = [[/index/users]]
  assert( result == html, result )
end

test.should_return_html_string_path = function()
  local result = helper:url( "/users/save" )
  local html   = [[/users/save]]
  assert( result == html, result )
end

test.should_return_html_string_path_with_query_string = function()
  local result = helper:url{ action = 'users', id = 1, cancel = false }
  local html   = [[/index/users?cancel=false&id=1]]
  assert( result == html, result )
end

return test
