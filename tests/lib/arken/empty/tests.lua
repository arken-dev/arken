local empty = require('arken.empty')
local test  = {}

test['should return false for 0'] = function()
  assert( empty(0) == false )
end

test['should return false for "0"'] = function()
  assert( empty("0") == false )
end

test['should return false for 1'] = function()
  assert( empty(1) == false )
end

test['should return false for boolean false'] = function()
  assert( empty(false) == false )
end

test['should return false for string false'] = function()
  assert( empty('false') == false )
end

test['should return true for empty table'] = function()
  assert( empty({}) == true )
end

test['should return false for table'] = function()
  assert( empty({'a'}) == false )
  assert( empty({a = 'b'}) == false )
end

test['should return true for empty string'] = function()
  assert( empty("") == true )
end

test['should return false for string'] = function()
  assert( empty(".") == false )
end

test['should return true for boolen false'] = function()
  assert( empty(false) == false )
end

return test
