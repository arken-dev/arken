local utf8 = require 'arken.utf8'
local test = {}

test['convert ALÇA'] = function()
  local str = 'ALÇA'
  local res = utf8.decode(str)
  assert( res == os.read('util/latin1/alca-upper.txt'), res )
end

test['convert alça'] = function()
  local str = 'alça'
  local res = utf8.decode(str)
  assert( res == os.read('util/latin1/alca.txt'), res )
end

test['convert ALÇAPÃO'] = function()
  local str = 'ALÇAPÃO'
  local res = utf8.decode(str)
  assert( res == os.read('util/latin1/alcapao-upper.txt'), res)
end

test['convert alçapão'] = function()
  local str = 'alçapão'
  local res = utf8.decode(str)
  assert( res == os.read('util/latin1/alcapao-lower.txt'), res )
end

test['should keep \\n'] = function()
  local res = utf8.decode('\n')
  assert( res == '\n', res )
end

test['should return empty string'] = function()
  local res = utf8.decode('')
  assert( res == '', res )
end

return test
