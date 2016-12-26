local Class = require('charon.oop.Class')
local test  = {}

test.should_require_class = function()
  local ActiveRecord = require('charon.ActiveRecord')
  assert( Class.retrieve('charon.ActiveRecord') == ActiveRecord )
end

test.should_require_class_without_charon_prefix = function()
  local ActiveRecord = require('charon.ActiveRecord')
  assert( Class.retrieve('ActiveRecord') == ActiveRecord )
end

test.should_error_if_class_not_exists = function()
  local result, message = pcall(Class.retrieve, 'NotExists')
  assert( result == false )
  assert( message:contains([[module 'NotExists' not found]]), message )
end

return test
