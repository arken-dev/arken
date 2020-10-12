local isblank = require('arken.isblank')
local test = {}

test.should_return_true_for_nil = function()
  assert( isblank(nil) == true )
end

test.should_return_true_for_empty_string = function()
  assert( isblank('') == true )
end

test.should_return_true_for_empty_table = function()
  assert( isblank({}) == true )
end

test.should_return_true_for_boolean_false = function()
  assert( isblank(false) == true )
end

test.should_return_false_for_string_with_space = function()
  assert( isblank(' ') == false )
end

test.should_return_false_for_table_with_values = function()
  assert( isblank({ id = 1 }) == false )
end

test.should_return_false_for_numbers = function()
  assert( isblank(1) == false )
end

return test
