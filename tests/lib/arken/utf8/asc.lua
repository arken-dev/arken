local utf8 = require 'arken.utf8'
local test = {}

test['convert ÀÁÂÃÄÅÈÉÊËÌÍÎÏÒÓÔÕÖÙÚÛÜ'] = function()
  local str = 'ÀÁÂÃÄÅÈÉÊËÌÍÎÏÒÓÔÕÖÙÚÛÜ'
  local res = utf8.asc(str)
  assert( res == 'AAAAAAEEEEIIIIOOOOOUUUU', res )
end

test['convert àáâãäåèéêëìíîïòóôõöøùúûü'] = function()
  local str = 'àáâãäåèéêëìíîïòóôõöøùúûü'
  local res = utf8.asc(str)
  assert( res == 'aaaaaaeeeeiiiioooooouuuu', str )
end

test['convert Çç'] = function()
  local str = 'Çç'
  local res = utf8.asc(str)
  assert( res == 'Cc', str )
end

test['convert Ýýÿ'] = function()
  local str = 'Ýýÿ'
  local res = utf8.asc(str)
  assert( res == 'Yyy', str )
end

test['convert Ññ'] = function()
  local str = 'Ññ'
  local res = utf8.asc(str)
  assert( res == 'Nn', str )
end

test['convert Ð'] = function()
  local str = 'Ð'
  local res = utf8.asc(str)
  assert( res == 'D', str )
end

test['convert ÀÁÂÃÄÅÆ with fallback *'] = function()
  local str = 'ÀÁÂÃÄÅÆ'
  local res = utf8.asc(str, '*')
  assert( res == 'AAAAAA*', str )
end

test['convert ÀÁÂÃÄÅÆ without fallback'] = function()
  local str = 'ÀÁÂÃÄÅÆ'
  local res = utf8.asc(str)
  assert( res == 'AAAAAA', str )
end

test['convert ÀÆÇ with fallback *'] = function()
  local str = 'ÀÆÇ'
  local res = utf8.asc(str, '*')
  assert( res == 'A*C', str )
end

test['convert ÀÆÇ with fallback *'] = function()
  local str = 'ÀÆÇ'
  local res = utf8.asc(str)
  assert( res == 'AC', str )
end

return test
