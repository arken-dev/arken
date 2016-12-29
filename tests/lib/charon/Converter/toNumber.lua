local Converter = require 'charon.Converter'
local test      = {}

test.should_return_1234 = function()
  local c = Converter.new{ total = '1234' }
  assert( c:toNumber('total') == 1234 )
end

test.should_return_0 = function()
  local c = Converter.new{ total = '0' }
  assert( c:toNumber('total') == 0 )
end

test.should_return_decimal = function()
  local c = Converter.new{ total = '1234.45' }
  assert( c:toNumber('total') == 1234.45 )
end

test.should_return_decimal_with_coma = function()
  local c = Converter.new{ total = '1.234,45' }
  assert( c:toNumber('total') == 1234.45 )
end

test.should_return_self_if_is_a_number = function()
  local c = Converter.new{ total = 1234.1234 }
  assert( c:toNumber('total') == 1234.1234 )
end

return test
