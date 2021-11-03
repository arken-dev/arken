local utf8 = require 'arken.utf8'
local test = {}

test['should return A'] = function()
  local str = 'ALÇA'
  local res = utf8.sub(str, 1, 1)
  assert(  res == 'A', res )
end

test['should return L'] = function()
  local str = 'ALÇA'
  local res = utf8.sub(str, 2, 2)
  assert( res == 'L', res )
end

test['should return Ç'] = function()
  local str = 'ALÇA'
  local res = utf8.sub(str, 3, 3)
  assert( res  == 'Ç', res )
end

test['should return ALÇ'] = function()
  local str = 'ALÇA'
  local res = utf8.sub(str, 1, 3)
  assert( res == 'ALÇ', res )
end

test['should return ALÇ'] = function()
  local str = 'ALÇA'
  local res = utf8.sub(str, 1, 3)
  assert( res == 'ALÇ', res )
end

test['should return ÇAPÃ'] = function()
  local str = 'ALÇAPÃO'
  local res = utf8.sub(str, 3, 6)
  assert( res == 'ÇAPÃ', res )
end

test['should return ÇÃO from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 2, 4)
  assert( res == 'ÇÃO', res )
  local res = utf8.sub(str, 2)
  assert( res == 'ÇÃO', res )
end

test['should return AÇÃ from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 1, 3)
  assert( res == 'AÇÃ', res )
end

test['should return ÇÃ from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 2, 3)
  assert( res == 'ÇÃ', res )
end

test['should return AÇ from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 1, 2)
  assert( res == 'AÇ', res )
end

test['should return A from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 1, 1)
  assert( res == 'A', res )
end

test['should return O from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 4, 4)
  assert( res == 'O', res )
end

test['should return Ç from AÇÃO'] = function()
  local str = 'AÇÃO'
  local res = utf8.sub(str, 2, 2)
  assert( res == 'Ç', res )
end

test['should return 3 first in ÁÉÍÓÚ'] = function()
  local str = 'ÁÉÍÓÚ'
  local res = utf8.sub(str, 1, 3)
  assert( res == 'ÁÉÍ', res )
end

test['should return 3 last in ÁÉÍÓÚ'] = function()
  local str = 'ÁÉÍÓÚ'
  local res = utf8.sub(str, 3, 5)
  assert( res == 'ÍÓÚ', res )
  local res = utf8.sub(str, 3)
  assert( res == 'ÍÓÚ', res )
end

return test
