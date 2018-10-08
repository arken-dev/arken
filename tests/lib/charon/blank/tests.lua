local blank = require('charon.blank')
local test  = {}

test['should return false for 0'] = function()
  assert( blank(0) == false )
end

test['should return false for "0"'] = function()
  assert( blank("0") == false )
end

test['should return false for 1'] = function()
  assert( blank(1) == false )
end

test['should return false for false boolean'] = function()
  assert( blank(false) == false )
end

test['should return false for string false'] = function()
  assert( blank('false') == false )
end

test['should return false for empty table'] = function()
  assert( blank({}) == false )
end

test['should return false for table'] = function()
  assert( blank({'a'}) == false )
  assert( blank({a = 'b'}) == false )
end

test['should return true for blank string'] = function()
  assert( blank("") == true )
end

test['should return false for string'] = function()
  assert( blank(".") == false )
end

test['should return false for boolen false'] = function()
  assert( blank(false) == false )
end

return test
