local Date = require('arken.chrono.Date')
local test = {}

test['should return true for equals dates'] = function()
  local d1 = Date.parse('2021-03-21')
  local d2 = Date.parse('2021-03-21')
  local res = ( d1 == d2 )
  assert( res == true  )
end

test['should return false for dt1 minor'] = function()
  local d1 = Date.parse('2021-03-20')
  local d2 = Date.parse('2021-03-21')
  local res = ( d1 == d2 )
  assert( res == false )
end

return test
