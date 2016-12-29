local Converter = require 'charon.Converter'
local test      = {}

test.should_return_true_for_blank_string = function()
  local c = Converter.new{ name = '' }
  assert( c:isEmpty('name') == true )
end

test.should_return_true_for_nil = function()
  local c = Converter.new{ name = nil }
  assert( c:isEmpty('name') == true )
end

test.should_return_false_for_space_string = function()
  local c = Converter.new{ name = ' ' }
  assert( c:isEmpty('name') == false )
end

test.should_return_false_for_empty_table = function()
  local c = Converter.new{ name = {} }
  assert( c:isEmpty('name') == true )
end

return test
