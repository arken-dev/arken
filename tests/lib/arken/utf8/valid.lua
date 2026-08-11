local utf8 = require 'arken.utf8'
local test = {}

test.should_return_true_for_valid_utf8 = function()
  assert( utf8.valid('ALÇA') == true )
  assert( utf8.valid('hello world') == true )
  assert( utf8.valid('') == true )
end

test.should_return_false_for_invalid_utf8 = function()
  assert( utf8.valid(string.char(0xFF)) == false )
  assert( utf8.valid(string.char(0xC0, 0x80)) == false ) -- overlong encoding
  assert( utf8.valid(string.char(0xED, 0xA0, 0x80)) == false ) -- surrogate half
  assert( utf8.valid(string.char(0xE2, 0x82)) == false ) -- sequência cortada
end

return test
