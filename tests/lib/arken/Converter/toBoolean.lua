local Converter = require 'arken.Converter'
local test      = {}

test.should_return_true_for_string_true = function()
  local c = Converter.new{ flag = 'true' }
  assert( c:toBoolean('flag') == true )
end

test.should_return_true_for_string_false = function()
  local c = Converter.new{ flag = 'false' }
  assert( c:toBoolean('flag') == false )
end

test.should_return_true_for_string_1 = function()
  local c = Converter.new{ flag = '1' }
  assert( c:toBoolean('flag') == true )
end

test.should_return_true_for_string_0 = function()
  local c = Converter.new{ flag = '0' }
  assert( c:toBoolean('flag') == false )
end

test.should_return_true_for_string_t = function()
  local c = Converter.new{ flag = 't' }
  assert( c:toBoolean('flag') == true )
end

test.should_return_true_for_string_f = function()
  local c = Converter.new{ flag = 'f' }
  assert( c:toBoolean('flag') == false )
end

test.should_return_nil_for_invalid_string = function()
  local c = Converter.new{ flag = 'unknown' }
  assert( c:toBoolean('flag') == nil )
end

return test
