local Date = require('arken.chrono.Date')
local test = {}

test['should return true for equals dates'] = function()
  local d1 = Date.parse('2021-03-21')
  local d2 = Date.parse('2021-03-21')
  assert( ( d1 < d2 ) == false )
  assert( ( d2 < d1 ) == false )
end

test['should return false for dt1 minor'] = function()
  local d1 = Date.parse('2021-03-20')
  local d2 = Date.parse('2021-03-21')
  assert( ( d1 < d2 ) == true )
  assert( ( d2 < d1 ) == false )
end

return test
