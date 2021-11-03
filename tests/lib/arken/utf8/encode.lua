local utf8 = require 'arken.utf8'
local test = {}

test['convert ALÇA'] = function()
  local str = os.read('util/latin1/ALCA.txt')
  local res = utf8.encode(str)
  assert( res == 'ALÇA', str )
end

test['convert alça'] = function()
  local str = os.read('util/latin1/alca.txt')
  local res = utf8.encode(str)
  assert( res == 'alça', str )
end

test['convert ALÇAPÃO'] = function()
  local str = os.read('util/latin1/ALCAPAO.txt')
  local res = utf8.encode(str)
  assert( res == 'ALÇAPÃO', str )
end

test['convert alçapão'] = function()
  local str = os.read('util/latin1/alcapao.txt')
  local res = utf8.encode(str)
  assert( res == 'alçapão', str )
end

test['should keep \\n'] = function()
  local res = utf8.encode('\n')
  assert( res == '\n', res )
end

test['should keep empty string'] = function()
  local res = utf8.encode('')
  assert( res == '', res )
end

return test
