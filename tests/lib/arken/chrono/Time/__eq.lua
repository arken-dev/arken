local Time = require('arken.chrono.Time')
local test = {}

test['should return true for equals dates'] = function()
  local d1 = Time.parse('2021-03-21')
  local d2 = Time.parse('2021-03-21')
  local res = ( d1 == d2 )
  assert( res == true )
end

test['should return false for dt1 minor'] = function()
  local d1 = Time.parse('2021-03-20')
  local d2 = Time.parse('2021-03-21')
  local res = ( d1 == d2 )
  assert( res == false )
end

test['should return true for equals dates with hours'] = function()
  local d1 = Time.parse('2021-03-21 12:30:12')
  local d2 = Time.parse('2021-03-21 12:30:12')
  local res = ( d1 == d2 )
  assert( res == true )
end

test['should return false for dt1 minor with hours'] = function()
  local d1 = Time.parse('2021-03-21 12:30:11')
  local d2 = Time.parse('2021-03-21 12:30:12')
  local res = ( d1 == d2 )
  assert( res == false )
end

return test
